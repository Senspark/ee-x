using UnityEngine;

namespace EETest {
    public struct ClickEvent : EE.IAnalyticsEvent {
        public string EventName => "click";
        public string button;
    }

    public class MainScene : MonoBehaviour {
        private bool _initialized;
        private EE.IAnalyticsManager _analyticsManager;
        private EE.IAudioManager _audioManager;
        private EE.ISceneLoader _sceneLoader;
        private EE.IAdsManager _adsManager;

        private void Awake() {
            EE.Utils.NoAwait(async () => {
                await ServiceUtils.Initialize();
                _analyticsManager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
                _audioManager = EE.ServiceLocator.Resolve<EE.IAudioManager>();
                _sceneLoader = EE.ServiceLocator.Resolve<EE.ISceneLoader>();
                var fileName = Application.platform == RuntimePlatform.Android
                    ? "Config/ads_config_android"
                    : "Config/ads_config_ios";
                var adsConfig = Resources.Load<TextAsset>(fileName).text;
                var adsManager = new EE.DefaultAdsManager(adsConfig);
                EE.Utils.NoAwait(adsManager.Initialize);
                _adsManager = adsManager;
                _initialized = true;
            });
        }

        public void OnAudioButtonPressed() {
            if (!_initialized) {
                return;
            }
            _audioManager.IsMusicEnabled = !_audioManager.IsMusicEnabled;
            _audioManager.IsSoundEnabled = !_audioManager.IsSoundEnabled;
        }

        public void OnOpenTestSuiteButtonPressed() {
            if (!_initialized) {
                return;
            }
            _analyticsManager.LogEvent(new ClickEvent {
                button = "open_test_suite"
            });
            _adsManager.OpenTestSuite();
        }

        public void OnTestBannerAdButtonPressed() {
            if (!_initialized) {
                return;
            }
            _analyticsManager.LogEvent(new ClickEvent {
                button = "test_banner_ad"
            });
            EE.Utils.NoAwait(async () => {
                var scene = await _sceneLoader.LoadScene<BannerAdScene>(nameof(BannerAdScene));
                scene.AdsManager = _adsManager;
                scene.Execute();
            });
        }

        public void OnTestFullScreenAdButtonPressed() {
            if (!_initialized) {
                return;
            }
            _analyticsManager.LogEvent(new ClickEvent {
                button = "test_full_screen_ad"
            });
            EE.Utils.NoAwait(async () => {
                var scene = await _sceneLoader.LoadScene<FullScreenAdScene>(nameof(FullScreenAdScene));
                scene.AdsManager = _adsManager;
            });
        }
    }
}