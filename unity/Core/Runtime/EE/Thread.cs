using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading;

using UnityEngine;

namespace EE {
    public static class Thread {
        private static readonly Queue<Action> _instantQueue = new Queue<Action>();
        private static SpinLock _instantLock = new SpinLock();

        private static int _queueCounter;
        private static readonly Dictionary<int, Action> _delayedQueue = new Dictionary<int, Action>();
        private static SpinLock _delayedLock = new SpinLock();

        private static void PushInstantRunnable(Action runnable) {
            var lockTaken = false;
            try {
                _instantLock.Enter(ref lockTaken);
                _instantQueue.Enqueue(runnable);
            } finally {
                if (lockTaken) {
                    _instantLock.Exit(false);
                }
            }
        }

        private static Action PopInstantRunnable() {
            var lockTaken = false;
            try {
                _instantLock.Enter(ref lockTaken);
                var runnable = _instantQueue.Dequeue();
                return runnable;
            } finally {
                if (lockTaken) {
                    _instantLock.Exit(false);
                }
            }
        }

        private static int PushDelayedRunnable(Action runnable) {
            var lockTaken = false;
            try {
                _delayedLock.Enter(ref lockTaken);
                _delayedQueue.Add(++_queueCounter, runnable);
                return _queueCounter;
            } finally {
                if (lockTaken) {
                    _delayedLock.Exit(false);
                }
            }
        }

        private static Action PopDelayedRunnable(int key) {
            var lockTaken = false;
            try {
                _delayedLock.Enter(ref lockTaken);
                if (_delayedQueue.TryGetValue(key, out var runnable)) {
                    _delayedQueue.Remove(key);
                    return runnable;
                }
            } finally {
                if (lockTaken) {
                    _delayedLock.Exit(false);
                }
            }
            return null;
        }

#if UNITY_ANDROID
        private static readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.ThreadKt");

        private static bool ee_isMainThread() {
            return _clazz.CallStatic<bool>("ee_isMainThread");
        }

        private static bool ee_runOnMainThread() {
            return _clazz.CallStatic<bool>("ee_runOnMainThread");
        }

        private static void ee_runOnMainThreadDelayed(int key, float delay) {
            _clazz.CallStatic("ee_runOnMainThreadDelayed", key, delay);
        }
#else // UNITY_ANDROID
        [DllImport("__Internal")]
        private static extern bool ee_isMainThread();

        [DllImport("__Internal")]
        private static extern bool ee_runOnMainThread();

        [DllImport("__Internal")]
        private static extern void ee_runOnMainThreadDelayed(int key, float delay);
#endif // UNITY_ANDROID

        private static void ee_runOnMainThreadCallback() {
            PopInstantRunnable()();
        }

        private static void ee_runOnMainThreadDelayedCallback(int key) {
            PopDelayedRunnable(key)();
        }

        public static bool IsMainThread() {
            return ee_isMainThread();
        }

        public static bool RunOnMainThread(Action runnable) {
            if (IsMainThread()) {
                runnable();
                return true;
            }
            PushInstantRunnable(runnable);
            return ee_runOnMainThread();
        }

        public static void RunOnMainThreadDelayed(Action runnable, float delay) {
            var key = PushDelayedRunnable(runnable);
            ee_runOnMainThreadDelayed(key, delay);
        }
    }
}