using System.Collections.Generic;

namespace EE.Internal {
    internal class NullAnalyticsParser : IAnalyticsParser {
        public List<IAnalyticsEvent> Parse(IAnalyticsEvent analyticsEvent) {
            var events = new List<IAnalyticsEvent>();
            if (AnalyticsUtils.IsLibraryEvent(analyticsEvent.EventName)) {
                // Ignored.
            } else {
                events.Add(analyticsEvent);
            }
            return events;
        }
    }
}