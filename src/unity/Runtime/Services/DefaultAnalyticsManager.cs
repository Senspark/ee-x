using System.Threading.Tasks;

namespace EE {
    public class DefaultAnalyticsManager : IAnalyticsManager {
        private readonly IAnalyticsManager _analyticsManager;

        public Task<bool> Initialize() {
            return _analyticsManager.Initialize();
        }

        public void Destroy() {
            _analyticsManager.Destroy();
        }

        public void PushScreen(string screenName) {
            _analyticsManager.PushScreen(screenName);
        }

        public void PopScreen() {
            _analyticsManager.PopScreen();
        }

        public void PopAllScreens() {
            _analyticsManager.PopAllScreens();
        }

        public void LogEvent(string name) {
            // _analyticsManager.LogEvent(name);
        }

        public void LogEvent<T>(T analyticsEvent) where T : IAnalyticsEvent {
            
        }
    }
}