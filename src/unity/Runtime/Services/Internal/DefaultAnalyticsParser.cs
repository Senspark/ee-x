namespace EE.Internal {
    internal class DefaultAnalyticsParser : IAnalyticsParser {
        public bool IsValid(string eventName) {
            return eventName.StartsWith("ee_");
        }
    }
}