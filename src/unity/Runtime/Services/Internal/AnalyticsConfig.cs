using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

using SimpleJSON;

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

            public MapperManager(JSONNode node) {
                _mappers = new Dictionary<string, Dictionary<string, string>>();
                if (node.HasKey("mappers")) {
                    foreach (var item in node["mappers"]) {
                        var mapper = new Dictionary<string, string>();
                        foreach (var mapItem in item.Value) {
                            mapper[mapItem.Key] = mapItem.Value.Value;
                        }
                        _mappers[item.Key] = mapper;
                    }
                }
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

            public EventManager(JSONNode node) {
                _events = new List<(IFromEvent, IToEvent)>();
                if (node.HasKey("events")) {
                    foreach (var item in node["events"]) {
                        var from = new FromEvent(item.Value["from"]);
                        var to = new ToEvent(item.Value["to"]);
                        _events.Add((from, to));
                    }
                }
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

            public FromEvent(JSONNode node) {
                _name = node.HasKey("name")
                    ? AnalyticsUtils.MakeLibraryEvent(node["name"].Value)
                    : null;
                _parameters = new Dictionary<string, string>();
                if (node.HasKey("parameters")) {
                    var parameters = node["parameters"];
                    foreach (var item in parameters) {
                        _parameters[item.Key] = item.Value.Value;
                    }
                }
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

            public ToEvent(JSONNode node) {
                Assert.IsTrue(node.HasKey("name"));
                _nameEvaluator = new Evaluator(node["name"].Value);
                _parameterEvaluators = new Dictionary<string, IEvaluator>();
                if (node.HasKey("parameters")) {
                    foreach (var item in node["parameters"]) {
                        _parameterEvaluators[item.Key] = new Evaluator(item.Value);
                    }
                }
            }

            public IAnalyticsEvent Map(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                var name = (string) _nameEvaluator.Evaluate(mapperManager, analyticsEvent);
                var parameters = _parameterEvaluators.ToDictionary(
                    item => item.Key,
                    item => item.Value.Evaluate(mapperManager, analyticsEvent));
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
            var node = JSON.Parse(text);
            return Parse(node);
        }

        private static AnalyticsConfig Parse(JSONNode node) {
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