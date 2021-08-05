using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class FirebaseAnalyticsManager : IAnalyticsManager {
        private readonly IFirebaseAnalyticsImpl _impl;
        private readonly Stack<string> _screens;
        private readonly Queue<Action> _pendingActions;
        private Task<bool> _initializer;
        private bool _initialized;

        public FirebaseAnalyticsManager() {
            _impl = new FirebaseAnalyticsImpl();
            _screens = new Stack<string>();
            _pendingActions = new Queue<Action>();
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
            var result = await FirebaseManager.Initialize();
            if (!result) {
                _initialized = false;
                return false;
            }
            _initialized = true;
            while (_pendingActions.Count > 0) {
                var action = _pendingActions.Dequeue();
                action();
            }
            return true;
        }

        public void PushScreen(string screenName) {
            if (!_initialized) {
                _pendingActions.Enqueue(() => PushScreen(screenName));
                return;
            }
            _screens.Push(screenName);
            _impl.SetCurrentScreen(screenName, null);
        }

        public void PopScreen() {
            if (!_initialized) {
                _pendingActions.Enqueue(PopScreen);
                return;
            }
            if (_screens.Count == 0) {
                return;
            }
            _screens.Pop();
            if (_screens.Count == 0) {
                _impl.SetCurrentScreen(null, null);
            } else {
                var screenName = _screens.Peek();
                _impl.SetCurrentScreen(screenName, null);
            }
        }

        public void PopAllScreens() {
            if (!_initialized) {
                _pendingActions.Enqueue(PopAllScreens);
                return;
            }
            _screens.Clear();
            _impl.SetCurrentScreen(null, null);
        }

        public void LogEvent(string name) {
            if (!_initialized) {
                _pendingActions.Enqueue(() => LogEvent(name));
                return;
            }
            _impl.LogEvent(name);
        }

        public void LogEvent(IAnalyticsEvent analyticsEvent) {
            if (!_initialized) {
                _pendingActions.Enqueue(() => LogEvent(analyticsEvent));
                return;
            }
            var parameters = analyticsEvent.Parameters
                .Select(item => (item.Key, item.Value))
                .ToArray();
            _impl.LogEvent(analyticsEvent.EventName, parameters);
        }
    }
}