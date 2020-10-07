//
//  AppLovinRewardedAdListener.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

import AppLovinSDK

private let kTag = "\(AppLovinRewardedAdListener.self)"
private let kPrefix = "AppLovinBridge"
private let kOnRewardedAdLoaded = "\(kPrefix)OnRewardedAdLoaded"
private let kOnRewardedAdFailedToLoad = "\(kPrefix)OnRewardedAdFailedToLoad"
private let kOnRewardedAdClicked = "\(kPrefix)OnRewardedAdClicked"
private let kOnRewardedAdClosed = "\(kPrefix)OnRewardedAdClosed"

internal class AppLovinRewardedAdListener:
    NSObject, ALAdLoadDelegate, ALAdDisplayDelegate, ALAdRewardDelegate, ALAdVideoPlaybackDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _isLoaded = false
    private var _rewarded = false

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
        _bridge.callCpp(kOnRewardedAdLoaded)
    }

    func adService(_ adService: ALAdService, didFailToLoadAdWithError code: Int32) {
        _logger.debug("\(kTag): \(#function): code \(code)")
        _bridge.callCpp(kOnRewardedAdFailedToLoad, "\(code)")
    }

    func ad(_ ad: ALAd, wasDisplayedIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _isLoaded = false
    }

    func videoPlaybackBegan(in ad: ALAd) {
        _logger.debug("\(kTag): \(#function)")
    }

    func ad(_ ad: ALAd, wasClickedIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _bridge.callCpp(kOnRewardedAdClicked)
    }

    func videoPlaybackEnded(in ad: ALAd, atPlaybackPercent percentPlayed: NSNumber, fullyWatched wasFullyWatched: Bool) {
        _logger.debug("\(kTag): \(#function)")
    }

    func rewardValidationRequest(for ad: ALAd, didSucceedWithResponse response: [AnyHashable: Any]) {
        _logger.debug("\(kTag): \(#function)")
        _rewarded = true
    }

    func rewardValidationRequest(for ad: ALAd, wasRejectedWithResponse response: [AnyHashable: Any]) {
        _logger.debug("\(kTag): \(#function)")
    }

    func rewardValidationRequest(for ad: ALAd, didExceedQuotaWithResponse response: [AnyHashable: Any]) {
        _logger.debug("\(kTag): \(#function)")
    }

    func rewardValidationRequest(for ad: ALAd, didFailWithError responseCode: Int) {
        _logger.debug("\(kTag): \(#function): code \(responseCode)")
    }

    func ad(_ ad: ALAd, wasHiddenIn view: UIView) {
        _logger.debug("\(kTag): \(#function)")
        _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
    }
}
