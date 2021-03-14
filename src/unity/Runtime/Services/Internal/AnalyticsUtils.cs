using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal static class AnalyticsUtils {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static string MakeEvent(string name) {
            return $"ee_{name}";
        }

        public static AnalyticsEventImpl ParseParameter<T>(T analyticsEvent) where T : IAnalyticsEvent {
            var type = typeof(T);
            var parameters = new Dictionary<string, object>();
            var infos = type.GetMembers(
                BindingFlags.Instance |
                BindingFlags.Public |
                BindingFlags.GetField |
                BindingFlags.GetProperty);
            foreach (var info in infos) {
                var attribute =
                    Attribute.GetCustomAttribute(info, typeof(AnalyticsParameterAttribute)) as
                        AnalyticsParameterAttribute;
                if (attribute == null) {
                    continue;
                }
                var name = attribute.Name ?? info.Name;
                object value = null;
                switch (info) {
                    case FieldInfo item:
                        value = item.GetValue(analyticsEvent);
                        break;
                    case PropertyInfo item:
                        value = item.GetValue(analyticsEvent);
                        break;
                    default:
                        Assert.IsFalse(true);
                        break;
                }
                parameters[name] = value;
            }
            return new AnalyticsEventImpl {
                EventName = analyticsEvent.EventName,
                Parameters = parameters
            };
        }
    }
}