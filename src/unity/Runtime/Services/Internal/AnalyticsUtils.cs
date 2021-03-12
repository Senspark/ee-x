using System.Collections.Generic;
using System.Linq;

namespace EE.Internal {
    internal static class AnalyticsUtils {
        public static List<(string, object)> ParseParameter<T>(T analyticsEvent) where T : IAnalyticsEvent {
            var type = typeof(T);
            var fields = type.GetFields();
            var parameters = fields.Select(item => {
                var name = item.Name;
                var value = item.GetValue(analyticsEvent);
                return (name, value);
            });
            return parameters.ToList();
        }
    }
}