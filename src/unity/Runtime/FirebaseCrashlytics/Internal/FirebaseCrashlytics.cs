using System;

namespace EE.Internal {
    using Destroyer = Action;

    internal class FirebaseCrashlytics : IFirebaseCrashlytics {
        private const string kTag = nameof(FirebaseCrashlytics);
        private const string kPrefix = "FirebaseCrashlyticsBridge";
        private const string kLog = kPrefix + "Log";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;

        public FirebaseCrashlytics(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            _destroyer();
        }

        public void Log(string message) {
            _bridge.Call(kLog, message);
        }
    }
}