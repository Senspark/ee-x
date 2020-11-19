using System.Threading.Tasks;

namespace EE {
    public interface IAppLovin : IPlugin {
        Task<bool> Initialize(string key);
        void SetVerboseLogging(bool enabled);
        void SetMuted(bool enabled);
        IRewardedAd CreateRewardedAd();
    }
}