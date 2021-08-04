# Multi Ad
## Configuration
Go to **Assets/Senspark EE-x/Settings** and enable:

- AdColony plugin
- AdMob plugin with mediation
- Facebook Ads plugin
- Iron Source plugin with mediation
- Unity Ads plugin

## Basic usage
Initializes with JSON config
```csharp
var config = Resources.Load<TextAsset>("config_path").text;
var adsManager = new EE.DefaultAdsManager(config);
EE.Utils.NoAwait(async () => {
    await adsManager.Initialize();
});
```

Normal banner ad
```csharp
// Show ad.
adsManager.BannerAd.IsVisible = true;

// Set ad position and anchor.
var (viewWidth, viewHeight) = EE.Platform.GetViewSize();
adsManager.BannerAd.Anchor = (0.5f, 0.5f);
adsManager.BannerAd.Position = (viewWidth * 0.5f, viewHeight * 0.5f);
```

Rectangle banner ad
```csharp
// Show ad.
adsManager.RectangleAd.IsVisible = true;

// Set ad position and anchor.
var (viewWidth, viewHeight) = EE.Platform.GetViewSize();
adsManager.RectangleAd.Anchor = (0.5f, 0.5f);
adsManager.RectangleAd.Position = (viewWidth * 0.5f, viewHeight * 0.5f);

// Set ad size
var widthAd = Platform.GetDensity() * 300;
var heightAd = Platform.GetDensity() * 250;
adsManager.RectangleAd.Size = (widthAd, heightAd);
```

App open ad
```csharp
var result = await adsManager.AppOpenAd.Show();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```csharp
var result = await adsManager.InterstitialAd.Show();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```csharp
var result = await adsManager.RewardedInterstitialAd.Show();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```csharp
var result = await adsManager.RewardedAd.Show();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```