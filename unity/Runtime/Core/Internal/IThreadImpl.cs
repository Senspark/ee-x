namespace EE.Internal {
    internal interface IThreadImpl {
        bool IsMainThread();
        bool RunOnMainThread();
        void RunOnMainThreadDelayed(int key, float delay);
    }
}