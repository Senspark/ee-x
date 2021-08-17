namespace EE {
    public interface IAppsFlyer : IPlugin {
        void Initialize(string devKey, string iosAppId);
        void StartTracking();
        string GetDeviceId();
        void SetDebugEnabled(bool enabled);
        void SetStopTracking(bool enabled);
    }
}