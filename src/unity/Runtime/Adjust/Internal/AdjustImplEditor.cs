using System.Threading.Tasks;

namespace EE.Internal {
    public class AdjustImplEditor : IAdjust {
        public void Destroy() {
        }

        public void Initialize(AdjustConfig config) {
        }

        public void SetEnabled(bool enabled) {
        }

        public Task<string> GetAdvertisingIdentifier() {
            return Task.FromResult("");
        }

        public string GetDeviceIdentifier() {
            return "";
        }

        public void SetPushToken(string token) {
        }

        public void TrackEvent(string token) {
        }
    }
}