using System.Collections.Generic;

namespace EE.Internal {
    internal interface IAnalyticsParser {
        List<IAnalyticsEvent> Parse(IAnalyticsEvent analyticsEvent);
    }
}