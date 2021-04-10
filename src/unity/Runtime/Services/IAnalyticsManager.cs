using System.Collections.Generic;

using EE.Internal;

using UnityEngine.Scripting;

namespace EE {
    public interface IAnalyticsEvent {
        string EventName { get; }
        Dictionary<string, object> Parameters { get; }
    }

    public abstract class DynamicAnalyticsEvent : IAnalyticsEvent {
        public abstract string EventName { get; }

        public Dictionary<string, object> Parameters => AnalyticsUtils.ParseParameter(this);
    }

    [Preserve]
    [Service("EE" + nameof(IAnalyticsManager))]
    public interface IAnalyticsManager : IService {
        void PushScreen(string screenName);
        void PopScreen();
        void PopAllScreens();
        void LogEvent(string name);
        void LogEvent(IAnalyticsEvent analyticsEvent);
    }
}