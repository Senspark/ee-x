#if UNITY_IOS
using System;
using System.Runtime.InteropServices;

using AOT;

namespace EE.Internal {
    using RunOnMainThreadCallback = Action;
    using RunOnMainThreadDelayedCallback = Action<int>;

    internal class ThreadImplIos : IThreadImpl {
        private static RunOnMainThreadCallback _runOnMainThreadCallback;
        private static RunOnMainThreadDelayedCallback _runOnMainThreadDelayedCallback;

        [DllImport("__Internal")]
        private static extern bool ee_isMainThread();

        [DllImport("__Internal")]
        private static extern bool ee_runOnMainThread();

        [DllImport("__Internal")]
        private static extern void ee_runOnMainThreadDelayed(int key, float delay);

        [DllImport("__Internal")]
        private static extern void ee_initializeThread(IntPtr instantPointer, IntPtr delayedPointer);

        [MonoPInvokeCallback(typeof(RunOnMainThreadCallback))]
        private static void ee_runOnMainThreadCallback() {
            _runOnMainThreadCallback();
        }

        [MonoPInvokeCallback(typeof(RunOnMainThreadDelayedCallback))]
        private static void ee_runOnMainThreadDelayedCallback(int key) {
            _runOnMainThreadDelayedCallback(key);
        }

        public ThreadImplIos(RunOnMainThreadCallback callback1, RunOnMainThreadDelayedCallback callback2) {
            var del1 = new RunOnMainThreadCallback(ee_runOnMainThreadCallback);
            var pointer1 = Marshal.GetFunctionPointerForDelegate(del1);
            var del2 = new RunOnMainThreadDelayedCallback(ee_runOnMainThreadDelayedCallback);
            var pointer2 = Marshal.GetFunctionPointerForDelegate(del2);
            ee_initializeThread(pointer1, pointer2);
            _runOnMainThreadCallback = callback1;
            _runOnMainThreadDelayedCallback = callback2;
        }

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
#endif // UNITY_IOS