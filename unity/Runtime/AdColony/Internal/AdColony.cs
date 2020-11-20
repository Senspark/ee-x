using System.Threading.Tasks;

namespace EE.Internal {
    internal class AdColony : IAdColony {
        private const string kPrefix = "AdColonyBridge";
        private const string kInitialize = kPrefix + "Initialize";

        private readonly IMessageBridge _bridge;

        public AdColony(IMessageBridge bridge) {
            _bridge = bridge;
        }

        public void Destroy() {
        }

        public async Task<bool> Initialize(string appId) {
            var response = await _bridge.CallAsync(kInitialize, appId);
            await Thread.SwitchToLibraryThread();
            return Utils.ToBool(response);
        }
    }
}