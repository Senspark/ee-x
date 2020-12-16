namespace EE.Internal {
    internal class FirebasePerformance : IFirebasePerformance {
        private readonly IMessageBridge _bridge;

        public FirebasePerformance(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public void Destroy() {
        }

        public bool IsDataCollectionEnabled { get; set; }

        public IFirebasePerformanceTrace NewTrace(string name) {
            throw new System.NotImplementedException();
        }
    }
}