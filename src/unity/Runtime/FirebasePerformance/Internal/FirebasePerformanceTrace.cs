namespace EE.Internal {
    internal class FirebasePerformanceTrace : IFirebasePerformanceTrace {
        private readonly IMessageBridge _bridge;
        private readonly FirebasePerformance _plugin;
        private readonly string _name;

        public FirebasePerformanceTrace(
            IMessageBridge bridge,
            FirebasePerformance plugin,
            string name) {
            _bridge = bridge;
            _plugin = plugin;
            _name = name;
        }

        public void Start() {
            throw new System.NotImplementedException();
        }

        public void Stop() {
            throw new System.NotImplementedException();
        }

        public void PutMetric(string metricName, long value) {
            throw new System.NotImplementedException();
        }

        public void IncrementMetric(string metricName, long incrementBy) {
            throw new System.NotImplementedException();
        }

        public long GetLongMetric(string metricName) {
            throw new System.NotImplementedException();
        }
    }
}