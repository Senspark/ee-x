using System;
using System.Collections.Generic;

using EE.Internal;

namespace EE {
    using PluginCreator = Func<IMessageBridge, ILogger, Action, IPlugin>;

    public static class PluginManager {
        private static readonly Dictionary<Type, (string, PluginCreator, PluginCreator)> _pluginInfo
            = new Dictionary<Type, (string, PluginCreator, PluginCreator)> {
                [typeof(IAdColony)] = ("AdColony",
                    (bridge, logger, destroyer) => new AdColony(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new AdColonyImplEditor()),
                [typeof(IAdjust)] = ("Adjust",
                    (bridge, logger, destroyer) => new Adjust(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new AdjustImplEditor()),
                [typeof(IAdMob)] = ("AdMob",
                    (bridge, logger, destroyer) => new AdMob(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new AdMobImplEditor()),
                [typeof(IAppsFlyer)] = ("AppsFlyer",
                    (bridge, logger, destroyer) => new AppsFlyer(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new AppsFlyerImplEditor()),
                [typeof(IFacebookAds)] = ("FacebookAds",
                    (bridge, logger, destroyer) => new FacebookAds(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new FacebookAdsImplEditor()),
                [typeof(IIronSource)] = ("IronSource",
                    (bridge, logger, destroyer) => new IronSource(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new IronSourceImplEditor()),
                [typeof(IUnityAds)] = ("UnityAds",
                    (bridge, logger, destroyer) => new UnityAds(bridge, logger, destroyer),
                    (bridge, logger, destroyer) => new UnityAdsImplEditor())
            };

        private static readonly IPluginManagerImpl _impl =
#if UNITY_EDITOR || UNITY_STANDALONE_OSX
            new PluginManagerImplEditor();
#elif UNITY_ANDROID
            new PluginManagerImplAndroid();
#elif UNITY_IOS
            new PluginManagerImplIos();
#endif

        private static readonly Dictionary<Type, IPlugin> _plugins = new Dictionary<Type, IPlugin>();
        private static Logger _logger;
        private static IMessageBridge _bridge;

        public static void InitializePlugins() {
            _logger = new Logger("ee-x");
            _impl.InitializePlugins("2.10.3");
            _bridge = new MessageBridge();
            Thread.Initialize();
            Platform.Initialize(_bridge);
        }

        public static T CreatePlugin<T>() where T : IPlugin {
            var type = typeof(T);
            if (_plugins.TryGetValue(type, out var plugin)) {
                return (T) plugin;
            }
            var (name, constructor, editorConstructor) = _pluginInfo[type];
            _impl.AddPlugin(name);
#if UNITY_EDITOR || UNITY_STANDALONE_OSX
            var currentConstructor = editorConstructor;
#else
            var currentConstructor = constructor;
#endif
            var instance = (T) currentConstructor(_bridge, _logger, () => {
                _impl.RemovePlugin(name);
                _plugins.Remove(type);
            });
            _plugins.Add(type, instance);
            return instance;
        }

        public static void SetLogLevel(LogLevel level) {
            _logger.LogLevel = level;
            _impl.SetLogLevel(level);
        }

        public static object GetActivity() {
            return _impl.GetActivity();
        }

        public static void SetActivity(object activity) {
            _impl.SetActivity(activity);
        }
    }
}