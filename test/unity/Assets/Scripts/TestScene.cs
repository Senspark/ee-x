using System.Threading;
using System.Threading.Tasks;

using UnityEngine;

namespace EETest {
    public class TestScene : MonoBehaviour {
        private void Awake() {
            Application.SetStackTraceLogType(LogType.Log, StackTraceLogType.None);
            TestCore();
            TestAdMob();
        }

        private void TestCore() {
            Debug.Log($"CurrentThreadId = {Thread.CurrentThread.ManagedThreadId}");
            Debug.Log($"InitializePlugins = {EE.PluginManager.InitializePlugins()}");
            Debug.Log($"IsMainThread = {EE.Thread.IsMainThread()}");

            EE.Thread.RunOnMainThread(() => { //
                Debug.Log($"MainThreadId = {Thread.CurrentThread.ManagedThreadId}");
            });

            Debug.Log($"GetApplicationId = {EE.Platform.GetApplicationId()}");
            Debug.Log($"GetApplicationName = {EE.Platform.GetApplicationName()}");
            Debug.Log($"GetVersionName = {EE.Platform.GetVersionName()}");
            Debug.Log($"GetVersionCode = {EE.Platform.GetVersionCode()}");
            Debug.Log($"IsTablet = {EE.Platform.IsTablet()}");
            Debug.Log($"GetDensity = {EE.Platform.GetDensity()}");
            var (viewWidth, viewHeight) = EE.Platform.GetViewSize();
            var (screenWidth, screenHeight) = EE.Platform.GetScreenSize();
            Debug.Log($"View size = {viewWidth} {viewHeight}");
            Debug.Log($"Screen size = {screenWidth} {screenHeight}");
            Debug.Log($"Screen native size = {Screen.width} {Screen.height}");
            Debug.Log($"Screen DPI = {Screen.dpi}");

            EE.Utils.NoAwait(async () => {
                var result = await EE.Platform.GetDeviceId();
                Debug.Log($"GetDeviceId = {result}");
            });

            EE.Utils.NoAwait(async () => {
                var result = await EE.Platform.TestConnection("www.google.com", 1.0f);
                Debug.Log($"TestConnection = {result}");
            });

            EE.Utils.NoAwait(async () => {
                var result = await EE.Platform.GetInstallReferrer();
                Debug.Log(
                    $"GetInstallReferrer: raw = {result.raw} " +
                    $"utm_source = {result.utm_source} " +
                    $"utm_medium = {result.utm_medium} " +
                    $"utm_term = {result.utm_term}" +
                    $"utm_content = {result.utm_content} " +
                    $"utm_campaign = {result.utm_campaign}");
            });
        }

        private void TestAdjust() {
            var config = new EE.AdjustConfig()
                .SetEnvironment(EE.AdjustEnvironment.Sandbox);
            var adjust = EE.PluginManager.CreatePlugin<EE.IAdjust>();
            adjust.Initialize(config);
        }

        private void TestAdMob() {
            var plugin = EE.PluginManager.CreatePlugin<EE.IAdMob>();
            EE.Utils.NoAwait(async () => { //
                await plugin.Initialize();
                TestAdMobBannerAd(plugin);
                TestAdMobInterstitialAd(plugin);
            });
        }

        private void TestAdMobBannerAd(EE.IAdMob plugin) {
            EE.Utils.NoAwait(async () => {
                var ad = plugin.CreateBannerAd(
                    "ca-app-pub-2101587572072038/6118633101",
                    EE.AdMobBannerAdSize.Normal);
                var (width, height) = ad.Size;
                Debug.Log($"Ad size = {width} x {height}");
                ad.IsVisible = false;
                EE.Utils.NoAwait(async () => {
                    while (true) {
                        Debug.Log($"Load begin");
                        var result = await ad.Load();
                        Debug.Log($"Load end: {result}");
                        if (result) {
                            ad.IsVisible = true;
                            break;
                        }
                        await Task.Delay(5000);
                    }
                });
                while (true) {
                    ad.Anchor = (0, 0);
                    ad.Position = (0, 0);
                    await Task.Delay(2000);
                    ad.Anchor = (1, 0);
                    ad.Position = (Screen.width, 0);
                    await Task.Delay(2000);
                    ad.Anchor = (0.5f, 0.5f);
                    ad.Position = (Screen.width / 2f, Screen.height / 2f);
                    await Task.Delay(2000);
                }
            });
        }

        private void TestAdMobInterstitialAd(EE.IAdMob plugin) {
            EE.Utils.NoAwait(async () => {
                var ad = plugin.CreateInterstitialAd("ca-app-pub-2101587572072038/4796977108");
                while (true) {
                    Debug.Log($"Load begin");
                    var loadResult = await ad.Load();
                    Debug.Log($"Load end: {loadResult}");
                    var showResult = await ad.Show();
                    Debug.Log($"Show end: {showResult}");
                    await Task.Delay(5000);
                }
            });
        }
    }
}