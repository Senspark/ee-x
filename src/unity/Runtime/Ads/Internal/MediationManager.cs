namespace EE.Internal {
    internal class MediationManager {
        public static MediationManager Instance { get; } = new MediationManager();
        public IAsyncHelper<FullScreenAdResult> AdDisplayer { get; } = new AsyncHelper<FullScreenAdResult>();
    }
}