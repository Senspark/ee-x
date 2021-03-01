using System.IO;
using System.Threading.Tasks;

using EE.Internal;

namespace EE {
    public class FirebaseLogManager : ILogManager {
        private readonly IFirebaseCrashlyticsImpl _impl;
        private Task<bool> _initializer;
        private bool _initialized;

        public FirebaseLogManager() {
            _impl = new FirebaseCrashlyticsImpl();
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
            _impl.Log(fullMessage);
        }
    }
}