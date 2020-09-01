using System;

using UnityEngine;

namespace EE.Internal {
    using RunOnMainThreadCallback = Action;
    using RunOnMainThreadDelayedCallback = Action<int>;

    internal class ThreadImplAndroid : IThreadImpl {
        private class Callback : AndroidJavaProxy {
            private static RunOnMainThreadCallback _runOnMainThreadCallback;
            private static RunOnMainThreadDelayedCallback _runOnMainThreadDelayedCallback;

            public Callback(RunOnMainThreadCallback callback1, RunOnMainThreadDelayedCallback callback2)
                : base("com.ee.internal.UnityThread$Callback") {
                _runOnMainThreadCallback = callback1;
                _runOnMainThreadDelayedCallback = callback2;
            }

            private void runOnMainThreadCallback() {
                _runOnMainThreadCallback();
            }

            private void runOnMainThreadDelayedCallback(int key) {
                _runOnMainThreadDelayedCallback(key);
            }
        }

        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.UnityThread");

        public ThreadImplAndroid(RunOnMainThreadCallback callback1, RunOnMainThreadDelayedCallback callback2) {
            _clazz.CallStatic("initialize", new Callback(callback1, callback2));
        }

        public bool IsMainThread() {
            return _clazz.CallStatic<bool>("isMainThread");
        }

        public bool RunOnMainThread() {
            return _clazz.CallStatic<bool>("runOnMainThread");
        }

        public void RunOnMainThreadDelayed(int key, float delay) {
            _clazz.CallStatic("runOnMainThreadDelayed", key, delay);
        }
    }
}