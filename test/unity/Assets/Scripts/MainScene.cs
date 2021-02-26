using UnityEngine;

namespace EETest {
    public struct AnalyticsTestEvent : EE.IAnalyticsEvent {
        public string EventName => "test_event";
        public string string_value;
        public int int_value;
        public double double_value;
    }

    public class MainScene : MonoBehaviour {
        private EE.IAnalyticsManager _analyticsManager;
        private EE.ISceneLoader _sceneLoader;
        private EE.IAdsManager _adsManager;

        private void Awake() {
            EE.Utils.NoAwait(async () => {
                await ServiceUtils.Initialize();
                _analyticsManager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
                _sceneLoader = EE.ServiceLocator.Resolve<EE.ISceneLoader>();
                _analyticsManager.LogEvent(new AnalyticsTestEvent {
                    string_value = "a",
                    int_value = 2,
                    double_value = 3.0
                });
                var fileName = Application.platform == RuntimePlatform.Android
                    ? "Config/ads_config_android"
                    : "Config/ads_config_ios";
                var adsConfig = Resources.Load<TextAsset>(fileName).text;
                var adsManager = new EE.DefaultAdsManager(adsConfig);
                EE.Utils.NoAwait(adsManager.Initialize);
                _adsManager = adsManager;
            });
        }

        public void OnTestBannerAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                var scene = await _sceneLoader.LoadScene<BannerAdScene>(nameof(BannerAdScene));
                scene.AdsManager = _adsManager;
                scene.Execute();
            });
        }

        public void OnTestFullScreenAdButtonPressed() {
            EE.Utils.NoAwait(async () => {
                var scene = await _sceneLoader.LoadScene<FullScreenAdScene>(nameof(FullScreenAdScene));
                scene.AdsManager = _adsManager;
            });
        }
    }
}