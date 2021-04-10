using System.Collections.Generic;

namespace EE.Internal {
    internal class DefaultAnalyticsParser : IAnalyticsParser {
        private readonly AnalyticsConfig _config;

        public DefaultAnalyticsParser(string configJson) {
            _config = AnalyticsConfig.Parse(configJson);
        }

        public List<IAnalyticsEvent> Parse(IAnalyticsEvent analyticsEvent) {
            if (AnalyticsUtils.IsLibraryEvent(analyticsEvent.EventName)) {
                return _config.Map(analyticsEvent);
            }
            return new List<IAnalyticsEvent> {
                analyticsEvent
            };
        }
    }
}