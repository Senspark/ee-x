using System.Collections.Generic;

using EE.Internal;

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

    public static class PluginManager {
        private static readonly Dictionary<Plugin, string> _pluginNames = new Dictionary<Plugin, string> {
            [Plugin.AdMob] = "AdMob",
            [Plugin.AppLovin] = "AppLovin",
            [Plugin.AppsFlyer] = "AppsFlyer",
            [Plugin.Facebook] = "Facebook",
            [Plugin.FacebookAds] = "FacebookAds",
            [Plugin.IronSource] = "IronSource",
            [Plugin.Play] = "Play",
            [Plugin.UnityAds] = "UnityAds",
            [Plugin.Vungle] = "Vungle"
        };

        private static readonly IPluginManagerImpl _impl =
#if UNITY_EDITOR
            new PluginManagerImplEditor();
#elif UNITY_ANDROID
            new PluginManagerImplAndroid();
#elif UNITY_IOS
            new PluginManagerImplIos();
#endif

        public static bool InitializePlugins() {
            if (!_impl.InitializePlugins()) {
                return false;
            }
            var bridge = MessageBridge.Instance;
            Platform.RegisterHandlers(bridge);
            return true;
        }

        public static object GetActivity() {
            return _impl.GetActivity();
        }

        public static void SetActivity(object activity) {
            _impl.SetActivity(activity);
        }

        public static bool AddPlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            return _impl.AddPlugin(name);
        }

        public static bool RemovePlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            return _impl.RemovePlugin(name);
        }
    }
}