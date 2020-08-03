namespace EE {
    public static class Utils {
        public static string ToString(bool value) {
            return value ? "true" : "false";
        }

        public static bool ToBool(string value) {
            return value == "true";
        }
    }
}