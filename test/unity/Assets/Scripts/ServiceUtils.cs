using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;

namespace EETest {
    public static class ServiceUtils {
        public static async Task Initialize() {
            EE.PluginManager.InitializePlugins();

            var dataManager = new EE.DefaultDataManager();
            var audioManager = new EE.DefaultAudioManager(dataManager, new Dictionary<Enum, EE.IAudioInfo>());

            var fileName = Application.platform == RuntimePlatform.Android
                ? "Config/ads_config_android"
                : "Config/ads_config_ios";
            var adsConfig = Resources.Load<TextAsset>(fileName).text;
            var remoteConfig = Resources.Load<TextAsset>("Config/remote_config").text;

            var adsManager = new EE.DefaultAdsManager(adsConfig);

#if UNITY_EDITOR
            var logManager = new EE.NullLogManager();
            var analyticsManager = new EE.NullAnalyticsManager(logManager);
            var remoteConfigManager = new EE.NullRemoteConfigManager(remoteConfig);
#else // UNITY_EDITOR
            var logManager = new EE.FirebaseLogManager();
            var analyticsManager = new EE.FirebaseAnalyticsManager();
            var remoteConfigManager = new EE.FirebaseRemoteConfigManager(dataManager, remoteConfig);
#endif // UNITY_EDITOR
            var sceneLoader = new EE.DefaultSceneLoader();

            await dataManager.Initialize();
            await audioManager.Initialize();
            await logManager.Initialize();
            await analyticsManager.Initialize();
            await sceneLoader.Initialize();
            await adsManager.Initialize();
            await remoteConfigManager.Initialize();

            EE.ServiceLocator.Provide(dataManager);
            EE.ServiceLocator.Provide(audioManager);
            EE.ServiceLocator.Provide(logManager);
            EE.ServiceLocator.Provide(analyticsManager);
            EE.ServiceLocator.Provide(sceneLoader);
            EE.ServiceLocator.Provide(adsManager);
            EE.ServiceLocator.Provide(remoteConfigManager);
        }
    }
}