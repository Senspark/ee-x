using System;

namespace EE.Internal {
    using Destroyer = Action;

    internal class FirebasePerformance : IFirebasePerformance {
        private const string kTag = nameof(FirebasePerformance);

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;

        public FirebasePerformance(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            _destroyer();
        }

        public bool IsDataCollectionEnabled { get; set; }

        public IFirebasePerformanceTrace NewTrace(string name) {
            throw new System.NotImplementedException();
        }
    }
}