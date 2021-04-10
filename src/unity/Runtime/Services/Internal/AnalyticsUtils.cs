using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal static class AnalyticsUtils {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static string MakeLibraryEvent(string name) {
            return $"ee_{name}";
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsLibraryEvent(string name) {
            return name.StartsWith("ee_");
        }

        public static string ParseAdFormat(AdFormat value) {
            switch (value) {
                case AdFormat.AppOpen: return "app_open";
                case AdFormat.Interstitial: return "interstitial";
                case AdFormat.RewardedInterstitial: return "rewarded_interstitial";
                case AdFormat.Rewarded: return "rewarded";
            }
            Assert.IsTrue(false);
            return "null";
        }

        public static string ParseAdResult(AdResult value) {
            switch (value) {
                case AdResult.Failed: return "failed";
                case AdResult.Canceled: return "canceled";
                case AdResult.Completed: return "completed";
                case AdResult.NotInitialized: return "not_initialized";
                case AdResult.NotConfigured: return "not_configured";
                case AdResult.Capped: return "capped";
                case AdResult.NoInternet: return "no_internet";
                case AdResult.NotLoaded: return "not_loaded";
            }
            Assert.IsTrue(false);
            return "null";
        }

        public static Dictionary<string, object> ParseParameter(IAnalyticsEvent analyticsEvent) {
            var type = analyticsEvent.GetType();
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
            return parameters;
        }
    }
}