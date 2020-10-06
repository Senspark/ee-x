using System;
using System.Collections.Generic;

using EE.Internal;

namespace EE {
    public enum Plugin {
        Adjust,
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
            [Plugin.Adjust] = "Adjust",
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

        private static readonly Dictionary<Type, (Plugin, Func<IMessageBridge, IPlugin>)> _pluginTypes
            = new Dictionary<Type, (Plugin, Func<IMessageBridge, IPlugin>)> {
                [typeof(IAdjust)] = (Plugin.Adjust, bridge => new Adjust(bridge)),
                [typeof(IAdMob)] = (Plugin.AdMob, bridge => new AdMob(bridge)),
                [typeof(IFacebookAds)] = (Plugin.FacebookAds, bridge => new FacebookAds(bridge)),
                [typeof(IIronSource)] = (Plugin.IronSource, bridge => new IronSource(bridge)),
                [typeof(IUnityAds)] = (Plugin.UnityAds, bridge => new UnityAds(bridge)),
            };

        private static readonly Dictionary<Plugin, IPlugin> _plugins = new Dictionary<Plugin, IPlugin>();

        private static readonly IPluginManagerImpl _impl =
#if UNITY_EDITOR
            new PluginManagerImplEditor();
#elif UNITY_ANDROID
            new PluginManagerImplAndroid();
#elif UNITY_IOS
            new PluginManagerImplIos();
#endif

        private static IMessageBridge _bridge;

        public static bool InitializePlugins() {
            if (!_impl.InitializePlugins()) {
                return false;
            }
            _bridge = MessageBridge.Instance;
            Thread.Initialize();
            Platform.RegisterHandlers(_bridge);
            return true;
        }

        public static T CreatePlugin<T>() where T : IPlugin {
            var type = typeof(T);
            var (plugin, constructor) = _pluginTypes[type];
            if (!AddPlugin(plugin)) {
                throw new Exception($"Can not add plugin {plugin}");
            }
            var instance = (T) constructor(_bridge);
            _plugins.Add(plugin, instance);
            return instance;
        }

        public static void DestroyPlugin<T>() where T : IPlugin {
            var type = typeof(T);
            var (plugin, pluginType) = _pluginTypes[type];
            if (!RemovePlugin(plugin)) {
                throw new Exception($"Can not remove plugin {plugin}");
            }
            _plugins.Remove(plugin);
        }

        public static void SetLogLevel(LogLevel level) {
            _impl.SetLogLevel(level);
        }

        public static object GetActivity() {
            return _impl.GetActivity();
        }

        public static void SetActivity(object activity) {
            _impl.SetActivity(activity);
        }

        private static bool AddPlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            return _impl.AddPlugin(name);
        }

        private static bool RemovePlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            return _impl.RemovePlugin(name);
        }
    }
}