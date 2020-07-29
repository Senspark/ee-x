namespace EE {
    public enum Plugin {
        AdMob,
        AppLovin,
        AppsFlyer,
        Facebook,
        FacebookAds,
        IronSource,
        Play,
        UnityAds,
        Vungle,
    }

    public class PluginManager {
        public static bool InitializePlugins() {
            return true;
        }

        public static bool AddPlugin(Plugin plugin) {
            return true;
        }

        public static bool RemovePlugin(Plugin plugin) {
            return true;
        }
    }
}