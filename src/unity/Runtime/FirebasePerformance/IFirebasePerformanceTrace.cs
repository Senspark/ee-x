namespace EE {
    public interface IFirebasePerformanceTrace {
        void Start();
        void Stop();
        void PutMetric(string metricName, long value);
        void IncrementMetric(string metricName, long incrementBy);
        long GetLongMetric(string metricName);
    }
}