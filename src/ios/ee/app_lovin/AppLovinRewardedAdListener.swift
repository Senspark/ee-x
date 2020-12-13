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
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = true
            self._bridge.callCpp(kOnRewardedAdLoaded)
        }
    }

    func adService(_ adService: ALAdService, didFailToLoadAdWithError code: Int32) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): code \(code)")
            self._bridge.callCpp(kOnRewardedAdFailedToLoad, "\(code)")
        }
    }

    func ad(_ ad: ALAd, wasDisplayedIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = false
        }
    }

    func videoPlaybackBegan(in ad: ALAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func ad(_ ad: ALAd, wasClickedIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(kOnRewardedAdClicked)
        }
    }

    func videoPlaybackEnded(in ad: ALAd, atPlaybackPercent percentPlayed: NSNumber, fullyWatched wasFullyWatched: Bool) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func rewardValidationRequest(for ad: ALAd, didSucceedWithResponse response: [AnyHashable: Any]) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._rewarded = true
        }
    }

    func rewardValidationRequest(for ad: ALAd, wasRejectedWithResponse response: [AnyHashable: Any]) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func rewardValidationRequest(for ad: ALAd, didExceedQuotaWithResponse response: [AnyHashable: Any]) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func rewardValidationRequest(for ad: ALAd, didFailWithError responseCode: Int) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): code \(responseCode)")
        }
    }

    func ad(_ ad: ALAd, wasHiddenIn view: UIView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(kOnRewardedAdClosed, Utils.toString(self._rewarded))
        }
    }
}
