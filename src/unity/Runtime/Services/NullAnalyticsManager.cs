using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EE {
    public class NullAnalyticsManager : IAnalyticsManager {
        private readonly ILogManager _logManager;
        private readonly Stack<string> _screens = new Stack<string>();
        private Task<bool> _initializer;
        private bool _initialized;

        public NullAnalyticsManager(ILogManager logManager) {
            _logManager = logManager;
        }

        public Task<bool> Initialize() => _initializer = _initializer ?? (_initializer = InitializeImpl(1f));

        public void Destroy() {
        }

        private async Task<bool> InitializeImpl(float timeOut) {
            await Task.WhenAny(
                Task.Delay((int) (timeOut * 1000)),
                InitializeImpl());
            return true;
        }

        private async Task<bool> InitializeImpl() {
            await _logManager.Initialize();
            _initialized = true;
            return true;
        }

        public void PushScreen(string screenName) {
            if (!_initialized) {
                return;
            }
            _screens.Push(screenName);
            _logManager.Log($"current_screen = {screenName}");
        }

        public void PopScreen() {
            if (!_initialized) {
                return;
            }
            if (_screens.Count == 0) {
                return;
            }
            _screens.Pop();
            if (_screens.Count == 0) {
                _logManager.Log($"current_screen = null");
            } else {
                var screenName = _screens.Peek();
                _logManager.Log($"current_screen = {screenName}");
            }
        }

        public void PopAllScreens() {
            if (!_initialized) {
                return;
            }
            _screens.Clear();
            _logManager.Log($"current_screen = null");
        }

        public void LogEvent(string name) {
            if (!_initialized) {
                return;
            }
            _logManager.Log($"[{name}]");
        }

        public void LogEvent<T>(T analyticsEvent) where T : IAnalyticsEvent {
            if (!_initialized) {
                return;
            }
            var type = typeof(T);
            var fields = type.GetFields();
            var tokens = new[] {
                $"[{analyticsEvent.EventName}]"
            };
            tokens = tokens.Concat(fields.Select(item => {
                var name = item.Name;
                var value = item.GetValue(analyticsEvent);
                return $"[{name}={value}]";
            })).ToArray();
            _logManager.Log($"{string.Join(" ", tokens)}");
        }
    }
}