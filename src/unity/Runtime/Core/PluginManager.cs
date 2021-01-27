using System;
using System.Collections.Generic;

using EE.Internal;

namespace EE {
    public static class PluginManager {
        private static readonly Dictionary<Type, (string, Func<IMessageBridge, ILogger, Action, IPlugin>)> _pluginInfo
            = new Dictionary<Type, (string, Func<IMessageBridge, ILogger, Action, IPlugin>)> {
                [typeof(IAdColony)] = ("AdColony", (bridge, logger, destroyer) => new AdColony(bridge)),
                [typeof(IAdjust)] = ("Adjust", (bridge, logger, destroyer) => new Adjust(bridge)),
                [typeof(IAdMob)] = ("AdMob", (bridge, logger, destroyer) => new AdMob(bridge)),
                [typeof(IFacebookAds)] = ("FacebookAds", (bridge, logger, destroyer) => new FacebookAds(bridge)),
                [typeof(IFirebaseCrashlytics)] = ("FirebaseCrashlytics",
                    (bridge, logger, destroyer) => new FirebaseCrashlytics(bridge)),
                [typeof(IFirebasePerformance)] = ("FirebasePerformance",
                    (bridge, logger, destroyer) => new FirebasePerformance(bridge)),
                [typeof(IIronSource)] = ("IronSource", (bridge, logger, destroyer) => new IronSource(bridge)),
                [typeof(IUnityAds)] = ("UnityAds", (bridge, logger, destroyer) => new UnityAds(bridge))
            };

        private static readonly IPluginManagerImpl _impl =
#if UNITY_EDITOR
            new PluginManagerImplEditor();
#elif UNITY_ANDROID
            new PluginManagerImplAndroid();
#elif UNITY_IOS
            new PluginManagerImplIos();
#endif

        private static Logger _logger;
        private static IMessageBridge _bridge;

        public static void InitializePlugins() {
            _logger = new Logger("ee-x");
            _impl.InitializePlugins("2.3.1");
            _bridge = new MessageBridge();
            Thread.Initialize();
            Platform.Initialize(_bridge);
        }

        public static T CreatePlugin<T>() where T : IPlugin {
            var type = typeof(T);
            var (name, constructor) = _pluginInfo[type];
            _impl.AddPlugin(name);
            var instance = (T) constructor(_bridge, _logger, () => _impl.RemovePlugin(name));
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