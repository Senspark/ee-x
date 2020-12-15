namespace EE {
    public interface IFirebasePerformance : IPlugin {
        bool IsDataCollectionEnabled { get; set; }
        IFirebasePerformanceTrace NewTrace(string name);
    }
}