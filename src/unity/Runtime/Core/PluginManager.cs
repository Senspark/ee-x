using System;
using System.Collections.Generic;

using EE.Internal;

namespace EE {
    public enum Plugin {
        AdColony,
        Adjust,
        AdMob,
        AppLovin,
        AppsFlyer,
        Facebook,
        FacebookAds,
        FirebaseCrashlytics,
        FirebasePerformance,
        IronSource,
        Play,
        UnityAds,
        Vungle,
    }

    public static class PluginManager {
        private static readonly Dictionary<Plugin, string> _pluginNames = new Dictionary<Plugin, string> {
            [Plugin.AdColony] = "AdColony",
            [Plugin.Adjust] = "Adjust",
            [Plugin.AdMob] = "AdMob",
            [Plugin.AppLovin] = "AppLovin",
            [Plugin.AppsFlyer] = "AppsFlyer",
            [Plugin.Facebook] = "Facebook",
            [Plugin.FacebookAds] = "FacebookAds",
            [Plugin.FirebaseCrashlytics] = "FirebaseCrashlytics",
            [Plugin.IronSource] = "IronSource",
            [Plugin.Play] = "Play",
            [Plugin.UnityAds] = "UnityAds",
            [Plugin.Vungle] = "Vungle"
        };

        private static readonly Dictionary<Type, (Plugin, Func<IMessageBridge, IPlugin>)> _pluginTypes
            = new Dictionary<Type, (Plugin, Func<IMessageBridge, IPlugin>)> {
                [typeof(IAdColony)] = (Plugin.AdColony, bridge => new AdColony(bridge)),
                [typeof(IAdjust)] = (Plugin.Adjust, bridge => new Adjust(bridge)),
                [typeof(IAdMob)] = (Plugin.AdMob, bridge => new AdMob(bridge)),
                [typeof(IFacebookAds)] = (Plugin.FacebookAds, bridge => new FacebookAds(bridge)),
                [typeof(IFirebaseCrashlytics)] =
                    (Plugin.FirebaseCrashlytics, bridge => new FirebaseCrashlytics(bridge)),
                [typeof(IFirebasePerformance)] =
                    (Plugin.FirebasePerformance, bridge => new FirebasePerformance(bridge)),
                [typeof(IIronSource)] = (Plugin.IronSource, bridge => new IronSource(bridge)),
                [typeof(IUnityAds)] = (Plugin.UnityAds, bridge => new UnityAds(bridge))
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

        public static void InitializePlugins() {
            _impl.InitializePlugins("2.1.2");
            _bridge = MessageBridge.Instance;
            Thread.Initialize();
            Platform.RegisterHandlers(_bridge);
        }

        public static T CreatePlugin<T>() where T : IPlugin {
            var type = typeof(T);
            var (plugin, constructor) = _pluginTypes[type];
            AddPlugin(plugin);
            var instance = (T) constructor(_bridge);
            _plugins.Add(plugin, instance);
            return instance;
        }

        public static void DestroyPlugin<T>() where T : IPlugin {
            var type = typeof(T);
            var (plugin, pluginType) = _pluginTypes[type];
            RemovePlugin(plugin);
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

        private static void AddPlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            _impl.AddPlugin(name);
        }

        private static void RemovePlugin(Plugin plugin) {
            var name = _pluginNames[plugin];
            _impl.RemovePlugin(name);
        }
    }
}