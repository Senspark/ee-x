using System;
using System.Threading.Tasks;

namespace EE.Internal {
    using RunOnMainThreadCallback = Action;
    using RunOnMainThreadDelayedCallback = Action<int>;

    internal class ThreadImplEditor : IThreadImpl {
        private static RunOnMainThreadCallback _runOnMainThreadCallback;
        private static RunOnMainThreadDelayedCallback _runOnMainThreadDelayedCallback;

        public ThreadImplEditor(RunOnMainThreadCallback callback1, RunOnMainThreadDelayedCallback callback2) {
            _runOnMainThreadCallback = callback1;
            _runOnMainThreadDelayedCallback = callback2;
        }

        public bool IsMainThread() {
            return true;
        }

        public bool RunOnMainThread() {
            _runOnMainThreadCallback();
            return true;
        }

        public void RunOnMainThreadDelayed(int key, float delay) {
            Task.Delay((int) (delay * 1000)).ContinueWith(async task => {
                await task;
                _runOnMainThreadDelayedCallback(key);
            }, TaskScheduler.FromCurrentSynchronizationContext());
        }
    }
}