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
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = true
            self._bridge.callCpp(kOnInterstitialAdLoaded)
        }
    }

    func adService(_ adService: ALAdService, didFailToLoadAdWithError code: Int32) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): code \(code)")
            self._bridge.callCpp(kOnInterstitialAdFailedToLoad, EEJsonUtils.convertDictionary(toString: [
                "code": code,
                "message": ""
            ]))
        }
    }

    func ad(_ ad: ALAd, wasDisplayedIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = false
        }
    }

    func ad(_ ad: ALAd, wasClickedIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(kOnInterstitialAdClicked)
        }
    }

    func ad(_ ad: ALAd, wasHiddenIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(kOnInterstitialAdClosed)
        }
    }
}
