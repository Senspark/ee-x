using System.Threading.Tasks;

namespace EE.Internal {
    internal class AdColonyImplEditor : IAdColony {
        public void Destroy() {
        }

        public Task<bool> Initialize(string appId, params string[] zoneIds) {
            return Task.FromResult(true);
        }
    }
}