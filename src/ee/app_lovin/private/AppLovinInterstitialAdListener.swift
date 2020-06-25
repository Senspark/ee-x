//
//  AppLovinInterstitialAdListener.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

import AppLovinSDK

private let kPrefix = "AppLovinBridge"
private let kOnInterstitialAdLoaded = "\(kPrefix)OnInterstitialAdLoaded"
private let kOnInterstitialAdFailedToLoad = "\(kPrefix)OnInterstitialAdFailedToLoad"
private let kOnInterstitialAdClicked = "\(kPrefix)OnInterstitialAdClicked"
private let kOnInterstitialAdClosed = "\(kPrefix)OnInterstitialAdClosed"

internal class AppLovinInterstitialAdListener:
    NSObject, ALAdLoadDelegate, ALAdDisplayDelegate {
    private let _logger = Logger("\(AppLovinInterstitialAdListener.self)")
    private let _bridge: IMessageBridge
    private var _isLoaded = false

    init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
    }

    var isLoaded: Bool {
        return _isLoaded
    }

    func adService(_ adService: ALAdService, didLoad ad: ALAd) {
        _logger.debug("\(#function)")
        _isLoaded = true
        _bridge.callCpp(kOnInterstitialAdLoaded)
    }

    func adService(_ adService: ALAdService, didFailToLoadAdWithError code: Int32) {
        _logger.debug("\(#function): code \(code)")
        _bridge.callCpp(kOnInterstitialAdFailedToLoad, "\(code)")
    }

    func ad(_ ad: ALAd, wasDisplayedIn view: UIView) {
        _logger.debug("\(#function)")
    }

    func ad(_ ad: ALAd, wasClickedIn view: UIView) {
        _logger.debug("\(#function)")
        _bridge.callCpp(kOnInterstitialAdClicked)
    }

    func ad(_ ad: ALAd, wasHiddenIn view: UIView) {
        _logger.debug("\(#function)")
        _isLoaded = false
        _bridge.callCpp(kOnInterstitialAdClosed)
    }
}
