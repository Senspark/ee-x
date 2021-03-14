using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

using SimpleJSON;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class AnalyticsConfig {
        private interface IMapperManager {
            string Find(string path, string key);
        }

        private class MapperManager : IMapperManager {
            private readonly Dictionary<string, Dictionary<string, string>> _mappers;

            public MapperManager(JSONNode node) {
                _mappers = new Dictionary<string, Dictionary<string, string>>();
                foreach (var item in node) {
                    var mapper = new Dictionary<string, string>();
                    foreach (var mapItem in item.Value) {
                        mapper[mapItem.Key] = mapItem.Value;
                    }
                    _mappers[item.Key] = mapper;
                }
            }

            public string Find(string path, string key) {
                const string prefix = "mappers";
                Assert.IsTrue(path.StartsWith(prefix));
                var entry = path.Substring(prefix.Length + 1);
                return _mappers[entry][key];
            }
        }

        private class NullMapperManager : IMapperManager {
            public string Find(string path, string key) {
                Assert.IsTrue(false);
                return null;
            }
        }

        private interface ITokenParser {
            string Evaluate(IAnalyticsEvent analyticsEvent);
        }

        private class ParameterEvaluator {
            private readonly string _format;
            private readonly List<Func<IMapperManager, IAnalyticsEvent, string>> _arguments;

            public ParameterEvaluator(string format) {
                _arguments = new List<Func<IMapperManager, IAnalyticsEvent, string>>();
                const string pattern = @"\{(.+?)\}";
                var index = 0;
                _format = Regex.Replace(format, pattern, match => {
                    _arguments.Add(MakeArgument(match.Value));
                    return $"{index++}";
                });
            }

            private Func<IMapperManager, IAnalyticsEvent, string> MakeArgument(string token) {
                const string pattern = @"mappers\.(.+?)\[event\.(.+?)\]";
                var matches = Regex.Matches(token, pattern);
                if (matches[1])
                if (matches[0])
            }

            public string Evaluate(IMapperManager mapperManager, IAnalyticsEvent analyticsEvent) {
                return string.Format(_format, _arguments.Select(item =>
                    item(mapperManager, analyticsEvent)));
            }
        }

        internal interface IEventManager {
        }

        private interface IFromEvent {
            bool IsMatched(AnalyticsEventImpl impl);
        }

        private interface IToEvent {
        }

        private class FromEvent : IFromEvent {
            private readonly string _name;
            private readonly Dictionary<string, string> _parameters;

            public FromEvent(JSONNode node) {
                _name = node.HasKey("name")
                    ? AnalyticsUtils.MakeEvent(node["name"])
                    : null;
                _parameters = new Dictionary<string, string>();
                if (node.HasKey("parameters")) {
                    var parameters = node["parameters"];
                    foreach (var item in parameters) {
                        _parameters[item.Key] = item.Value.Value;
                    }
                }
            }

            public bool IsMatched(AnalyticsEventImpl impl) {
                if (_name != null && _name == impl.EventName) {
                    return false;
                }
                foreach (var item in _parameters) {
                    if (!impl.Parameters.TryGetValue(item.Key, out var value)) {
                        return false;
                    }
                    if (value.Equals(item.Value)) {
                        return false;
                    }
                }
                return true;
            }
        }

        public static AnalyticsConfig Parse(string text) {
            var node = JSON.Parse(text);
            return Parse(node);
        }

        private static AnalyticsConfig Parse(JSONNode node) {
        }
    }
}