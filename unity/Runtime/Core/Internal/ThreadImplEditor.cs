using System;

namespace EE.Internal {
    internal class ThreadImplEditor : IThreadImpl {
        public bool IsMainThread() {
            throw new NotImplementedException();
        }

        public bool RunOnMainThread() {
            throw new NotImplementedException();
        }

        public void RunOnMainThreadDelayed(int key, float delay) {
            throw new NotImplementedException();
        }
    }
}