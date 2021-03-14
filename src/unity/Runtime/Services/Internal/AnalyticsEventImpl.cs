using System.Collections.Generic;

namespace EE.Internal {
    internal class AnalyticsEventImpl {
        public string EventName { get; set; }
        public Dictionary<string, object> Parameters { get; set; }
    }
}