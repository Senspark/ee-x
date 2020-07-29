using System;

namespace EE {
    public static class Thread {
        public static bool IsMainThread() {
            return true;
        }

        public static bool RunOnMainThread(Action runnable) {
            return true;
        }

        public static void RunOnMainThreadDelayed(Action runnable, float delay) {
        }
    }
}