using System;
using System.Collections.Generic;
using System.Threading;

using EE.Internal;

namespace EE {
    public static class Thread {
        private static readonly Queue<Action> _instantQueue = new Queue<Action>();
        private static SpinLock _instantLock = new SpinLock();

        private static int _queueCounter;
        private static readonly Dictionary<int, Action> _delayedQueue = new Dictionary<int, Action>();
        private static SpinLock _delayedLock = new SpinLock();

        private static Func<bool> _libraryThreadChecker;
        private static Func<Action, bool> _libraryThreadExecuter;

        private static readonly IThreadImpl _impl =
#if UNITY_EDITOR
            new ThreadImplEditor();
#elif UNITY_ANDROID
            new ThreadImplAndroid(ee_runOnMainThreadCallback, ee_runOnMainThreadDelayedCallback);
#elif UNITY_IOS
            new ThreadImplIos(ee_runOnMainThreadCallback, ee_runOnMainThreadDelayedCallback);
#endif

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

        private static void ee_runOnMainThreadCallback() {
            PopInstantRunnable()();
        }

        private static void ee_runOnMainThreadDelayedCallback(int key) {
            PopDelayedRunnable(key)();
        }

        public static bool IsLibraryThread() {
            return _libraryThreadChecker();
        }

        public static bool RunOnLibraryThread(Action runnable) {
            return _libraryThreadExecuter(runnable);
        }

        public static bool IsMainThread() {
            return _impl.IsMainThread();
        }

        public static bool RunOnMainThread(Action runnable) {
            if (IsMainThread()) {
                runnable();
                return true;
            }
            PushInstantRunnable(runnable);
            return _impl.RunOnMainThread();
        }

        public static void RunOnMainThreadDelayed(Action runnable, float delay) {
            var key = PushDelayedRunnable(runnable);
            _impl.RunOnMainThreadDelayed(key, delay);
        }
    }
}