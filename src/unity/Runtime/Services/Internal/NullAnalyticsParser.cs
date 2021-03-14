namespace EE.Internal {
    internal class NullAnalyticsParser : IAnalyticsParser {
        public AnalyticsEventImpl Parse<T>(T analyticsEvent) where T : IAnalyticsEvent {
            return analyticsEvent.EventName.StartsWith("ee_")
                ? null
                : AnalyticsUtils.ParseParameter(analyticsEvent);
        }
    }
}