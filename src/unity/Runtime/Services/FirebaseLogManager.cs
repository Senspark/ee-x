using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class FirebaseLogManager : ILogManager {
        private readonly IFirebaseCrashlyticsImpl _impl;
        private readonly Queue<Action> _pendingActions;
        private Task<bool> _initializer;
        private bool _initialized;

        public FirebaseLogManager() {
            _impl = new FirebaseCrashlyticsImpl();
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

        public void Log(
            string message = "",
            string memberName = "",
            string sourceFilePath = "",
            int sourceLineNumber = 0) {
            if (!_initialized) {
                _pendingActions.Enqueue(() => Log(message, memberName, sourceFilePath, sourceLineNumber));
                return;
            }
            var fileName = Path.GetFileName(sourceFilePath);
            var messageWithColon = message.Length == 0 ? "" : $": {message}";
            var fullMessage = $"{fileName}:{sourceLineNumber}: {memberName + messageWithColon}";
            _impl.Log(fullMessage);
        }
    }
}