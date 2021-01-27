using UnityEngine;

namespace EETest {
    public class MainScene : MonoBehaviour {
        private EE.ISceneLoader _sceneLoader;
        private EE.IAdsManager _adsManager;

        private void Awake() {
            ServiceUtils.Initialize();
            _sceneLoader = EE.ServiceLocator.Resolve<EE.ISceneLoader>();

            var fileName = Application.platform == RuntimePlatform.Android
                ? "Config/ads_config_android"
                : "Config/ads_config_ios";
            var adsConfig = Resources.Load<TextAsset>(fileName).text;
            var adsManager = new EE.DefaultAdsManager(adsConfig);
            EE.Utils.NoAwait(adsManager.Initialize);
            _adsManager = adsManager;
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