using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

using Jsonite;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class AnalyticsConfig {
        private class AnalyticsEventImpl : IAnalyticsEvent {
            public string EventName { get; set; }
            public Dictionary<string, object> Parameters { get; set; }
        }

        private interface IMapperManager {
            string Find(string path, string key);
        }

        private class MapperManager : IMapperManager {
            private readonly Dictionary<string, Dictionary<string, string>> _mappers;

            public MapperManager(JsonObject node) {
                var mappers = node.TryGetValue("mappers", out var value)
                    ? (JsonObject) value
                    : new JsonObject();
                _mappers = mappers.ToDictionary(entry => entry.Key,
                    entry => ((JsonObject) entry.Value).ToDictionary(
                        innerEntry => innerEntry.Key,
                        innerEntry => (string) innerEntry.Value));
            }

            public string Find(string entry, string key) {
                return _mappers[entry][key];
            }
        }

        private interface IEventManager {
            List<IAnalyticsEvent> Map(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent);
        }

        private class EventManager : IEventManager {
            private readonly List<(IFromEvent, IToEvent)> _events;

            public EventManager(JsonObject node) {
                var events = node.TryGetValue("events", out var value)
                    ? (JsonArray) value
                    : new JsonArray();
                _events = events.Select(entry => {
                    var obj = (JsonObject) entry;
                    var from = (IFromEvent) new FromEvent((JsonObject) obj["from"]);
                    var to = (IToEvent) new ToEvent((JsonObject) obj["to"]);
                    return (from, to);
                }).ToList();
            }

            public List<IAnalyticsEvent> Map(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                var events = new List<IAnalyticsEvent>();
                foreach (var (from, to) in _events) {
                    if (from.IsMatched(analyticsEvent)) {
                        events.Add(to.Map(mapperManager, analyticsEvent));
                    }
                }
                return events;
            }
        }

        private interface IFromEvent {
            bool IsMatched(IAnalyticsEvent analyticsEvent);
        }

        private class FromEvent : IFromEvent {
            private readonly string _name;
            private readonly Dictionary<string, string> _parameters;

            public FromEvent(JsonObject node) {
                _name = node.TryGetValue("name", out var name)
                    ? AnalyticsUtils.MakeLibraryEvent((string) name)
                    : null;
                var parameters = node.TryGetValue("parameters", out var value)
                    ? (JsonObject) value
                    : new JsonObject();
                _parameters = parameters.ToDictionary(
                    entry => entry.Key,
                    entry => (string) entry.Value);
            }

            public bool IsMatched(IAnalyticsEvent analyticsEvent) {
                if (_name != null && _name != analyticsEvent.EventName) {
                    return false;
                }
                foreach (var item in _parameters) {
                    if (!analyticsEvent.Parameters.TryGetValue(item.Key, out var value)) {
                        return false;
                    }
                    if (!value.Equals(item.Value)) {
                        return false;
                    }
                }
                return true;
            }
        }

        private interface IToEvent {
            IAnalyticsEvent Map(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent);
        }

        private class ToEvent : IToEvent {
            private readonly IEvaluator _nameEvaluator;
            private readonly Dictionary<string, IEvaluator> _parameterEvaluators;

            public ToEvent(JsonObject node) {
                Assert.IsTrue(node.ContainsKey("name"));
                _nameEvaluator = new Evaluator((string) node["name"]);
                var parameters = node.TryGetValue("parameters", out var value)
                    ? (JsonObject) value
                    : new JsonObject();
                _parameterEvaluators = parameters.ToDictionary(
                    entry => entry.Key,
                    entry => (IEvaluator) new Evaluator((string) entry.Value));
            }

            public IAnalyticsEvent Map(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                var name = (string) _nameEvaluator.Evaluate(mapperManager, analyticsEvent);
                var parameters = _parameterEvaluators.ToDictionary(
                    entry => entry.Key,
                    entry => entry.Value.Evaluate(mapperManager, analyticsEvent));
                return new AnalyticsEventImpl {
                    EventName = name,
                    Parameters = parameters
                };
            }
        }

        private interface IEvaluator {
            object Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent);
        }

        private class RawEvaluator : IEvaluator {
            private readonly string _token;

            public RawEvaluator(string token) {
                _token = token;
            }

            public object Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                return _token;
            }
        }

        private class EventEvaluator : IEvaluator {
            private readonly bool _useName;
            private readonly string _parameter;

            public static EventEvaluator ParseImpl(string token) {
                const string pattern = @"event\.(.+)";
                var matches = Regex.Matches(token, pattern);
                if (matches.Count == 0) {
                    return null;
                }
                var match = matches[0];
                return new EventEvaluator(match.Groups[1].Value);
            }

            private EventEvaluator(string path) {
                if (path == "name") {
                    _useName = true;
                } else {
                    _useName = false;
                    const string prefix = "parameters";
                    Assert.IsTrue(path.StartsWith(prefix));
                    _parameter = path.Substring(prefix.Length + 1);
                }
            }

            public object Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                return _useName
                    ? analyticsEvent.EventName
                    : analyticsEvent.Parameters[_parameter];
            }
        }

        private class MapperEvaluator : IEvaluator {
            private readonly string _path;
            private readonly IEvaluator _evaluator;

            public static MapperEvaluator ParseImpl(string token) {
                const string pattern = @"mappers\.(.+?)\[(.+?)\]";
                var matches = Regex.Matches(token, pattern);
                if (matches.Count == 0) {
                    return null;
                }
                var match = matches[0];
                var path = match.Groups[1].Value;
                var key = match.Groups[2].Value;
                return new MapperEvaluator(path, key);
            }

            public MapperEvaluator(string path, string key) {
                _path = path;
                _evaluator = EventEvaluator.ParseImpl(key);
            }

            public object Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                return mapperManager.Find(_path, (string) _evaluator.Evaluate(mapperManager, analyticsEvent));
            }
        }

        private class Evaluator : IEvaluator {
            private static IEvaluator ParseTokenEvaluator(string token) {
                var mapperEvaluator = MapperEvaluator.ParseImpl(token);
                if (mapperEvaluator != null) {
                    return mapperEvaluator;
                }
                var eventEvaluator = EventEvaluator.ParseImpl(token);
                if (eventEvaluator != null) {
                    return eventEvaluator;
                }
                return new RawEvaluator(token);
            }

            private readonly string _format;
            private readonly List<IEvaluator> _evaluators;

            public Evaluator(string format) {
                _evaluators = new List<IEvaluator>();
                const string pattern = @"\{(.+?)\}";
                var index = 0;
                _format = Regex.Replace(format, pattern, match => {
                    var token = match.Groups[1].Value;
                    var evaluator = ParseTokenEvaluator(token);
                    _evaluators.Add(evaluator);
                    return $"{{{index++}}}";
                });
            }

            public object Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                var arguments = _evaluators
                    .Select(item => item.Evaluate(mapperManager, analyticsEvent))
                    .ToArray();
                return string.Format(_format, arguments);
            }
        }

        private IMapperManager _mapperManager;
        private IEventManager _eventManager;

        public static AnalyticsConfig Parse(string text) {
            var node = (JsonObject) Json.Deserialize(text);
            return Parse(node);
        }

        private static AnalyticsConfig Parse(JsonObject node) {
            var mapperManager = new MapperManager(node);
            var eventManager = new EventManager(node);
            var result = new AnalyticsConfig {
                _mapperManager = mapperManager,
                _eventManager = eventManager
            };
            return result;
        }

        public List<IAnalyticsEvent> Map(IAnalyticsEvent analyticsEvent) {
            return _eventManager.Map(_mapperManager, analyticsEvent);
        }
    }
}