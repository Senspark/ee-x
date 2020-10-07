//
//  IronSourceBridge.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import Foundation

private let kTag = "\(IronSourceBridge.self)"
private let kPrefix = "IronSourceBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kHasInterstitialAd = "\(kPrefix)HasInterstitialAd"
private let kLoadInterstitialAd = "\(kPrefix)LoadInterstitialAd"
private let kShowInterstitialAd = "\(kPrefix)ShowInterstitialAd"
private let kHasRewardedAd = "\(kPrefix)HasRewardedAd"
private let kShowRewardedAd = "\(kPrefix)ShowRewardedAd"
private let kOnInterstitialAdLoaded = "\(kPrefix)OnInterstitialAdLoaded"
private let kOnInterstitialAdFailedToLoad = "\(kPrefix)OnInterstitialAdFailedToLoad"
private let kOnInterstitialAdFailedToShow = "\(kPrefix)OnInterstitialAdFailedToShow"
private let kOnInterstitialAdClicked = "\(kPrefix)OnInterstitialAdClicked"
private let kOnInterstitialAdClosed = "\(kPrefix)OnInterstitialAdClosed"
private let kOnRewardedAdLoaded = "\(kPrefix)OnRewardedAdLoaded"
private let kOnRewardedAdFailedToShow = "\(kPrefix)OnRewardedAdFailedToShow"
private let kOnRewardedAdClicked = "\(kPrefix)OnRewardedAdClicked"
private let kOnRewardedAdClosed = "\(kPrefix)OnRewardedAdClosed"

@objc(EEIronSourceBridge)
public class IronSourceBridge:
    NSObject, IPlugin, ISRewardedVideoDelegate, ISInterstitialDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initialized = false
    private var _isInterstitialAdLoaded = false
    private var _isRewardedAdLoaded = false
    private var _rewarded = false

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        Thread.runOnMainThread {
            if !self._initialized {
                return
            }
            // Cannot clear delegates.
        }
    }

    func registerHandlers() {
        _bridge.registerHandler(kInitialize) { message in
            self.initialize(message)
            return ""
        }
        _bridge.registerHandler(kLoadInterstitialAd) { _ in
            self.loadInterstitialAd()
            return ""
        }
        _bridge.registerHandler(kHasInterstitialAd) { _ in
            Utils.toString(self.hasInterstitialAd)
        }
        _bridge.registerHandler(kShowInterstitialAd) { message in
            self.showInterstitialAd(message)
            return ""
        }
        _bridge.registerHandler(kHasRewardedAd) { _ in
            Utils.toString(self.hasRewardedAd)
        }
        _bridge.registerHandler(kShowRewardedAd) { message in
            self.showRewardedAd(message)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    func initialize(_ appKey: String) {
        Thread.runOnMainThread {
            if self._initialized {
                return
            }
            IronSource.initWithAppKey(appKey, adUnits: [
                IS_REWARDED_VIDEO,
                IS_INTERSTITIAL
            ])
            IronSource.shouldTrackReachability(true)
            IronSource.setInterstitialDelegate(self)
            IronSource.setRewardedVideoDelegate(self)
            IronSource.setUserId(IronSource.advertiserId())
            self._initialized = true
        }
    }

    var hasInterstitialAd: Bool {
        return _isInterstitialAdLoaded
    }

    func loadInterstitialAd() {
        Thread.runOnMainThread {
            IronSource.loadInterstitial()
        }
    }

    func showInterstitialAd(_ adId: String) {
        Thread.runOnMainThread {
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            IronSource.showInterstitial(with: rootView, placement: adId)
        }
    }

    var hasRewardedAd: Bool {
        return _isRewardedAdLoaded
    }

    func showRewardedAd(_ adId: String) {
        Thread.runOnMainThread {
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            self._rewarded = false
            IronSource.showRewardedVideo(with: rootView, placement: adId)
        }
    }

    func handleRewardedAdResult() {
        _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
    }

    public func interstitialDidLoad() {
        _logger.debug("\(kTag): \(#function)")
        _isInterstitialAdLoaded = true
        _bridge.callCpp(kOnInterstitialAdLoaded)
    }

    public func interstitialDidFailToLoadWithError(_ error: Error) {
        _logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
        _bridge.callCpp(kOnInterstitialAdFailedToLoad, error.localizedDescription)
    }

    public func interstitialDidShow() {
        _logger.debug("\(kTag): \(#function)")
        _isRewardedAdLoaded = false
    }

    public func interstitialDidFailToShowWithError(_ error: Error) {
        _logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
        _bridge.callCpp(kOnInterstitialAdFailedToShow, error.localizedDescription)
    }

    public func interstitialDidOpen() {
        _logger.debug("\(kTag): \(#function)")
    }

    public func didClickInterstitial() {
        _logger.debug("\(kTag): \(#function)")
        _bridge.callCpp(kOnInterstitialAdClicked)
    }

    public func interstitialDidClose() {
        _logger.debug("\(kTag): \(#function)")
        _isInterstitialAdLoaded = false
        _bridge.callCpp(kOnInterstitialAdClosed)
    }

    public func rewardedVideoHasChangedAvailability(_ available: Bool) {
        _logger.debug("\(kTag): \(#function): \(available)")
        if available {
            _isRewardedAdLoaded = true
            _bridge.callCpp(kOnRewardedAdLoaded)
        }
    }

    public func rewardedVideoDidFailToShowWithError(_ error: Error) {
        _logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
        _bridge.callCpp(kOnRewardedAdFailedToShow, error.localizedDescription)
    }

    public func rewardedVideoDidOpen() {
        _logger.debug("\(kTag): \(#function)")
    }

    public func rewardedVideoDidStart() {
        _logger.debug("\(kTag): \(#function)")
    }

    public func didClickRewardedVideo(_ placementInfo: ISPlacementInfo) {
        _logger.debug("\(kTag): \(#function): \(placementInfo.placementName ?? "")")
        _bridge.callCpp(kOnRewardedAdClicked)
    }

    public func rewardedVideoDidEnd() {
        _logger.debug("\(kTag): \(#function)")
    }

    public func didReceiveReward(forPlacement placementInfo: ISPlacementInfo) {
        _logger.debug("\(kTag): \(#function): \(placementInfo.placementName ?? "")")
        _rewarded = true
    }

    public func rewardedVideoDidClose() {
        _logger.debug("\(kTag): \(#function)")
        // Note: The didReceiveReward and rewardedVideoDidClose events are
        // asynchronous.
        if _rewarded {
            // Already has result.
            handleRewardedAdResult()
        } else {
            Thread.runOnMainThreadDelayed(1.0) {
                self.handleRewardedAdResult()
            }
        }
    }
}
