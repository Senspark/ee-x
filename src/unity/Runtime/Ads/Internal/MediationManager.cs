namespace EE.Internal {
    internal class MediationManager {
        public static MediationManager Instance { get; } = new MediationManager();
        public IAsyncHelper<AdResult> AdDisplayer { get; } = new AsyncHelper<AdResult>();
    }
}