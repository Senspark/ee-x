using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class DefaultAnalyticsManager : IAnalyticsManager {
        private readonly IAnalyticsManager _analyticsManager;
        private readonly IAnalyticsParser _parser;

        public DefaultAnalyticsManager(IAnalyticsManager analyticsManager, string configJson) {
            _analyticsManager = analyticsManager;
            _parser = new DefaultAnalyticsParser(configJson);
        }

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
            _analyticsManager.LogEvent(name);
        }

        public void LogEvent(IAnalyticsEvent analyticsEvent) {
            var events = _parser.Parse(analyticsEvent);
            foreach (var item in events) {
                _analyticsManager.LogEvent(item);
            }
        }
    }
}