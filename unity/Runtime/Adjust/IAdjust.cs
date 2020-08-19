using System.Threading.Tasks;

namespace EE {
    public interface IAdjust : IPlugin {
        void Initialize(AdjustConfig config);
        void SetEnabled(bool enabled);
        Task<string> GetAdvertisingIdentifier();
        string GetDeviceIdentifier();
        void TrackEvent(string token);
    }
}