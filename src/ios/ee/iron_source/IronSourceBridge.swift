//
//  IronSourceBridge.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import Foundation
import RxSwift

private let kTag = "\(IronSourceBridge.self)"
private let kPrefix = "IronSourceBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kGetBannerAdSize = "\(kPrefix)GetBannerAdSize"
private let kCreateBannerAd = "\(kPrefix)CreateBannerAd"
private let kDestroyBannerAd = "\(kPrefix)DestroyBannerAd"
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
class IronSourceBridge: NSObject, IPlugin, ISRewardedVideoDelegate, ISInterstitialDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initialized = false
    private let _bannerHelper = IronSourceBannerHelper()
    private var _bannerAds: [String: IronSourceBannerAd] = [:]
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
        _bannerAds.values.forEach { $0.destroy() }
        _bannerAds.removeAll()
        // Cannot clear delegates.
    }

    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message, resolver in
            self.initialize(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kGetBannerAdSize) { message in
            let index = Int(message) ?? -1
            let size = self._bannerHelper.getSize(index: index)
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        _bridge.registerHandler(kCreateBannerAd) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let adId = dict["adId"] as? String,
                let index = dict["adSize"] as? Int
            else {
                assert(false, "Invalid argument")
                return ""
            }
            let adSize = self._bannerHelper.getAdSize(index)
            return Utils.toString(self.createBannerAd(adId, adSize))
        }
        _bridge.registerHandler(kDestroyBannerAd) { message in
            Utils.toString(self.destroyBannerAd(message))
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
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyBannerAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    func checkInitialized() {
        if !_initialized {
            assert(false, "Please call initialize() first")
        }
    }

    func initialize(_ appKey: String) -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                if self._initialized {
                    self._logger.info("\(kTag): \(#function): initialized")
                    single(.success(true))
                    return
                }
                IronSource.initWithAppKey(appKey, adUnits: [
                    IS_REWARDED_VIDEO,
                    IS_INTERSTITIAL,
                    IS_BANNER
                ])
                IronSource.shouldTrackReachability(true)
                IronSource.setInterstitialDelegate(self)
                IronSource.setRewardedVideoDelegate(self)
                IronSource.setUserId(IronSource.advertiserId())
                self._initialized = true
                self._logger.info("\(kTag): \(#function): done")
                single(.success(true))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    func createBannerAd(_ adId: String, _ adSize: ISBannerSize) -> Bool {
        checkInitialized()
        if _bannerAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = IronSourceBannerAd(_bridge, _logger, adId, adSize, _bannerHelper)
        _bannerAds[adId] = ad
        return true
    }

    func destroyBannerAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _bannerAds[adId] else {
            return false
        }
        ad.destroy()
        _bannerAds.removeValue(forKey: adId)
        return true
    }

    var hasInterstitialAd: Bool {
        return _isInterstitialAdLoaded
    }

    func loadInterstitialAd() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self.checkInitialized()
            IronSource.loadInterstitial()
        }
    }

    func showInterstitialAd(_ adId: String) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(adId)")
            self.checkInitialized()
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
            self._logger.debug("\(kTag): \(#function): id = \(adId)")
            self.checkInitialized()
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            self._rewarded = false
            IronSource.showRewardedVideo(with: rootView, placement: adId)
        }
    }

    func handleRewardedAdResult() {
        _isRewardedAdLoaded = false
        _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
    }

    public func interstitialDidLoad() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isInterstitialAdLoaded = true
            self._bridge.callCpp(kOnInterstitialAdLoaded)
        }
    }

    public func interstitialDidFailToLoadWithError(_ error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self._bridge.callCpp(kOnInterstitialAdFailedToLoad, error.localizedDescription)
        }
    }

    public func interstitialDidShow() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    public func interstitialDidFailToShowWithError(_ error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self._isInterstitialAdLoaded = false
            self._bridge.callCpp(kOnInterstitialAdFailedToShow, error.localizedDescription)
        }
    }

    public func interstitialDidOpen() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    public func didClickInterstitial() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(kOnInterstitialAdClicked)
        }
    }

    public func interstitialDidClose() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isInterstitialAdLoaded = false
            self._bridge.callCpp(kOnInterstitialAdClosed)
        }
    }

    public func rewardedVideoHasChangedAvailability(_ available: Bool) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(available)")
            if available {
                self._isRewardedAdLoaded = true
                self._bridge.callCpp(kOnRewardedAdLoaded)
            }
        }
    }

    public func rewardedVideoDidFailToShowWithError(_ error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self._isRewardedAdLoaded = false
            self._bridge.callCpp(kOnRewardedAdFailedToShow, error.localizedDescription)
        }
    }

    public func rewardedVideoDidOpen() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    public func rewardedVideoDidStart() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    public func didClickRewardedVideo(_ placementInfo: ISPlacementInfo) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(placementInfo.placementName ?? "")")
            self._bridge.callCpp(kOnRewardedAdClicked)
        }
    }

    public func rewardedVideoDidEnd() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    public func didReceiveReward(forPlacement placementInfo: ISPlacementInfo) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(placementInfo.placementName ?? "")")
            self._rewarded = true
        }
    }

    public func rewardedVideoDidClose() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            // Note: The didReceiveReward and rewardedVideoDidClose events are
            // asynchronous.
            if self._rewarded {
                // Already has result.
                self.handleRewardedAdResult()
            } else {
                Thread.runOnMainThreadDelayed(1.0) {
                    self.handleRewardedAdResult()
                }
            }
        }
    }
}
