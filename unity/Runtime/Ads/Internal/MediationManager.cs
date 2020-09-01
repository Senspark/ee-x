namespace EE.Internal {
    internal class MediationManager {
        public static MediationManager Instance { get; } = new MediationManager();
        public IAsyncHelper<bool> InterstitialAdDisplayer { get; } = new AsyncHelper<bool>();
        public IAsyncHelper<IRewardedAdResult> RewardedAdDisplayer { get; } = new AsyncHelper<IRewardedAdResult>();
    }
}