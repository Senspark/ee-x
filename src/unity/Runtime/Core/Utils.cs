using System;
using System.Threading.Tasks;

namespace EE {
    public static class Utils {
        public static string ToString(bool value) {
            return value ? "true" : "false";
        }

        public static bool ToBool(string value) {
            return value == "true";
        }

        public static void NoAwait(Func<Task> callable) {
            var task = callable();
            task.Forget();
        }
    }
}