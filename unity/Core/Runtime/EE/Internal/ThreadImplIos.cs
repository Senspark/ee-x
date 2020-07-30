using System.Runtime.InteropServices;

namespace EE.Internal {
    internal class ThreadImplIos : IThreadImpl {
        [DllImport("__Internal")]
        private static extern bool ee_isMainThread();

        [DllImport("__Internal")]
        private static extern bool ee_runOnMainThread();

        [DllImport("__Internal")]
        private static extern void ee_runOnMainThreadDelayed(int key, float delay);

        public bool IsMainThread() {
            return ee_isMainThread();
        }

        public bool RunOnMainThread() {
            return ee_runOnMainThread();
        }

        public void RunOnMainThreadDelayed(int key, float delay) {
            ee_runOnMainThreadDelayed(key, delay);
        }
    }
}