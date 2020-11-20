# Multi Ad
- Purpose: use ad waterfall to optimize monetization.
- Supported ad:
    + Use `EE.MultiAdView` for banner and native ads.
    + Use `EE.MultiInterstitialAd` for interstitial ads.
    + Use `EE.MultiRewardedAd` for rewarded ads.
- Use `AddItem` to configure ad waterfall.
- Samples:
```csharp
// Samples for using 4 ad networks:
// - AdMob.
// - Facebook Audience Network.
// - IronSource
// - Unity Ads

var admob = EE.PluginManager.CreatePlugin<EE.IAdMob>();
var facebook = EE.PluginManager.CreatePlugin<EE.IFacebookAds>();
var ironsource = EE.PluginManager.CreatePlugin<EE.IIronSource>();
var unity = EE.PluginManager.CreatePlugin<EE.IUnityAds>();

EE.Utils.NoAwait(async () => {
    await admob.Initialize();
    await facebook.Initialize();
    await ironsource.Initialize("app_id");
    await unity.Initialize("app_id", false);
});

// Create a banner ad.
var ad = new EE.MultiAdView()
    .AddItem(admob.CreateBannerAd("ad_id", EE.AdMobBannerAdSize.Normal))
    .AddItem(facebook.CreateBannerAd("ad_id", EE.FacebookBannerAdSize.BannerHeight50));

// Load the ad in background.
ad.Load();

// Show the ad.
ad.IsVisible = true;

// Create an interstitial ad.
var ad = new EE.MultiInterstitialAd()
    .AddItem(admob.CreateInterstitialAd("ad_id"))
    .AddItem(facebook.CreateInterstitialAd("ad_id"))
    .AddItem(ironsource.CreateInterstitialAd("ad_id"))
    .AddItem(unity.CreateInterstitialAd("ad_id"));

// Load and show the ad.
EE.Utils.NoAwait(async () => {
    await ad.Load();
    await ad.Show();
});

// Create a rewarded ad.
var ad = new EE.MultiRewardedAd()
    .AddItem(admob.CreateRewardedAd("ad_id"))
    .AddItem(facebook.CreateRewardedAd("ad_id"))
    .AddItem(ironsource.CreateRewardedAd("ad_id"))
    .AddItem(unity.CreateRewardedAd("ad_id"));

// Load and show the ad.
EE.Utils.NoAwait(async () => {
    await ad.Load();
    await ad.Show();
});
```