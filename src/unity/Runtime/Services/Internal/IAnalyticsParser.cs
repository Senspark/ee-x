namespace EE.Internal {
    internal interface IAnalyticsParser {
        AnalyticsEventImpl Parse<T>(T analyticsEvent) where T : IAnalyticsEvent;
    }
}