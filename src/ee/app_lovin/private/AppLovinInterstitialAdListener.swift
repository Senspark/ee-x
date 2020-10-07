//
//  AppLovinInterstitialAdListener.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

import AppLovinSDK

private let kTag = "\(AppLovinInterstitialAdListener.self)"
private let kPrefix = "AppLovinBridge"
private let kOnInterstitialAdLoaded = "\(kPrefix)OnInterstitialAdLoaded"
private let kOnInterstitialAdFailedToLoad = "\(kPrefix)OnInterstitialAdFailedToLoad"
private let kOnInterstitialAdClicked = "\(kPrefix)OnInterstitialAdClicked"
private let kOnInterstitialAdClosed = "\(kPrefix)OnInterstitialAdClosed"

internal class AppLovinInterstitialAdListener:
    NSObject, ALAdLoadDelegate, ALAdDisplayDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _isLoaded = false

    init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
    }

    var isLoaded: Bool {
        return _isLoaded
    }

    func adService(_ adService: ALAdService, didLoad ad: ALAd) {
        _logger.debug("\(kTag): \(#function)")
        _isLoaded = true
        _bridge.callCpp(kOnInterstitialAdLoaded)
    }

    func adService(_ adService: ALAdService, didFailToLoadAdWithError code: Int32) {
        _logger.debug("\(kTag): \(#function): code \(code)")
        _bridge.callCpp(kOnInterstitialAdFailedToLoad, "\(code)")
    }

    func ad(_ ad: ALAd, wasDisplayedIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _isLoaded = false
    }

    func ad(_ ad: ALAd, wasClickedIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _bridge.callCpp(kOnInterstitialAdClicked)
    }

    func ad(_ ad: ALAd, wasHiddenIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _bridge.callCpp(kOnInterstitialAdClosed)
    }
}
