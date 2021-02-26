using UnityEngine.Scripting;

namespace EE {
    public interface IAnalyticsEvent {
        string EventName { get; }
    }

    [Preserve]
    [Service("EE" + nameof(IAnalyticsManager))]
    public interface IAnalyticsManager : IService {
        void PushScreen(string screenName);
        void PopScreen();
        void PopAllScreens();
        void LogEvent<T>(T analyticsEvent) where T : IAnalyticsEvent;
    }
}