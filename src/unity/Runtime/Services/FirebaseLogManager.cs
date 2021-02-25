using System;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    public class FirebaseLogManager : ILogManager {
        private readonly Type _type;
        private readonly MethodInfo _method;
        private Task<bool> _initializer;
        private bool _initialized;

        public FirebaseLogManager() {
            _type = Type.GetType("Firebase.Crashlytics.Crashlytics, Firebase.Crashlytics");
            if (_type == null) {
                throw new MissingReferenceException("Cannot find FirebaseCrashlytics");
            }
            _method = _type.GetMethod("Log");
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
            return true;
        }

        public void Log(
            string message = "",
            string memberName = "",
            string sourceFilePath = "",
            int sourceLineNumber = 0) {
            if (!_initialized) {
                return;
            }
            var fileName = Path.GetFileName(sourceFilePath);
            var messageWithColon = message.Length == 0 ? "" : $": {message}";
            var fullMessage = $"{fileName}:{sourceLineNumber}: {memberName + messageWithColon}";
            _method.Invoke(null, new object[] {fullMessage});
        }
    }
}