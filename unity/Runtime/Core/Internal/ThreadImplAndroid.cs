using UnityEngine;

namespace EE.Internal {
    internal class ThreadImplAndroid : IThreadImpl {
        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.ThreadKt");

        public bool IsMainThread() {
            return _clazz.CallStatic<bool>("ee_isMainThread");
        }

        public bool RunOnMainThread() {
            return _clazz.CallStatic<bool>("ee_runOnMainThread");
        }

        public void RunOnMainThreadDelayed(int key, float delay) {
            _clazz.CallStatic("ee_runOnMainThreadDelayed", key, delay);
        }
    }
}