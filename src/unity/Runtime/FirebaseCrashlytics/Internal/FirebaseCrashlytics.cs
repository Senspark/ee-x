namespace EE.Internal {
    internal class FirebaseCrashlytics : IFirebaseCrashlytics {
        private const string kPrefix = "FirebaseCrashlyticsBridge";
        private const string kLog = kPrefix + "Log";

        private readonly IMessageBridge _bridge;

        public FirebaseCrashlytics(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public void Destroy() {
        }

        public void Log(string message) {
            _bridge.Call(kLog, message);
        }
    }
}