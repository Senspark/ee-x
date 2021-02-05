# AdMob
## Configuration
Go to **Assets/Senspark EE-x/Settings** and enable AdMob plugin

## Basic usage
Initialization
```csharp
var plugin = EE.PluginManager.CreatePlugin<EE.IAdMob>();
EE.Utils.NoAwait(async () => {
    var result = await plugin.Initialize();
    // Handle initialization result.
});
```

Creates and show a banner ad.
```csharp
var ad = plugin.CreateBannerAd("ad_id", EE.AdMobBannerAdSize.Normal);

// Load the ad in background.
ad.Load();

// Show the ad.
ad.IsVisible = true;
```

Creates and show an interstitial ad
```csharp
var ad = plugin.CreateInterstitialAd("ad_id");

// Load and show the ad.
EE.Utils.NoAwait(async () => {
    await ad.Load();
    await ad.Show();
});
```

Creates and show a rewarded ad
```csharp
var ad = plugin.CreateRewardedAd("ad_id");

// Load and show the ad.
EE.Utils.NoAwait(async () => {
    await ad.Load();
    await ad.Show();
});
```