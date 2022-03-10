//
//  UnityAds.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/25/20.
//

import RxSwift
import UnityAds

private let kTag = "\(UnityAdsBridge.self)"
private let kPrefix = "UnityAdsBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetDebugModeEnabled = "\(kPrefix)SetDebugModeEnabled"
private let kGetBannerAdSize = "\(kPrefix)GetBannerAdSize"
private let kCreateBannerAd = "\(kPrefix)CreateBannerAd"
private let kDestroyAd = "\(kPrefix)DestroyAd"
private let kHasRewardedAd = "\(kPrefix)HasRewardedAd"
private let kLoadRewardedAd = "\(kPrefix)LoadRewardedAd"
private let kShowRewardedAd = "\(kPrefix)ShowRewardedAd"
private let kOnLoaded = "\(kPrefix)OnLoaded"
private let kOnFailedToShow = "\(kPrefix)OnFailedToShow"
private let kOnClosed = "\(kPrefix)OnClosed"

private class InitializeDelegate: NSObject, UnityAdsInitializationDelegate {
    private let _onCompleted: () -> Void
    private let _onFailed: (UnityAdsInitializationError, String) -> Void

    public init(_ onCompleted: @escaping () -> Void,
                _ onFailed: @escaping (UnityAdsInitializationError, String) -> Void) {
        _onCompleted = onCompleted
        _onFailed = onFailed
    }

    func initializationComplete() {
        _onCompleted()
    }

    func initializationFailed(_ error: UnityAdsInitializationError, withMessage message: String) {
        _onFailed(error, message)
    }
}

private class LoadDelegate: NSObject, UnityAdsLoadDelegate {
    private let _onLoaded: (String) -> Void
    private let _onFailed: (String) -> Void

    public init(_ onLoaded: @escaping (String) -> Void,
                _ onFailed: @escaping (String) -> Void) {
        _onLoaded = onLoaded
        _onFailed = onFailed
    }

    func unityAdsAdLoaded(_ placementId: String) {
        _onLoaded(placementId)
    }

    func unityAdsAdFailed(toLoad placementId: String, withError error: UnityAdsLoadError, withMessage message: String) {
        _onFailed(placementId)
    }
}

private class ShowDelegate: NSObject, UnityAdsShowDelegate {
    private let _onFailedToShow: (String) -> Void
    private let _onClosed: (Bool) -> Void
    
    public init(_ onFailedToShow: @escaping (String) -> Void,
                _ onClosed: @escaping (Bool) -> Void) {
        _onFailedToShow = onFailedToShow
        _onClosed = onClosed
    }
    
    func unityAdsShowComplete(_ placementId: String, withFinish state: UnityAdsShowCompletionState) {
        if (state == UnityAdsShowCompletionState.showCompletionStateCompleted) {
            _onClosed(true)
        } else {
            _onClosed(false)
        }
    }
    
    func unityAdsShowFailed(_ placementId: String, withError error: UnityAdsShowError, withMessage message: String) {
        _onFailedToShow(message)
    }
    
    func unityAdsShowStart(_ placementId: String) {
        // TODO.
    }
    
    func unityAdsShowClick(_ placementId: String) {
        // TODO.
    }
}

@objc(EEUnityAdsBridge)
class UnityAdsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false
    private let _bannerHelper = UnityBannerHelper()
    private var _ads: [String: IAd] = [:]
    private var _displayed = false
    private var _loadedAdIds: Set<String> = []

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        _ads.values.forEach { $0.destroy() }
        _ads.removeAll()
    }

    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let gameId = dict["gameId"] as? String,
                let testModeEnabled = dict["testModeEnabled"] as? Bool
            else {
                assert(false, "Invalid argument")
                return
            }
            self.initialize(gameId, testModeEnabled)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kSetDebugModeEnabled) { message in
            self.setDebugModeEnabled(Utils.toBool(message))
            return ""
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
        _bridge.registerHandler(kDestroyAd) { message in
            Utils.toString(self.destroyAd(message))
        }
        _bridge.registerHandler(kHasRewardedAd) { message in
            Utils.toString(self.hasRewardedAd(message))
        }
        _bridge.registerAsyncHandler(kLoadRewardedAd) { message, resolver in
            self.loadRewardedAd(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kShowRewardedAd) { message in
            self.showRewardedAd(message)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetDebugModeEnabled)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    func initialize(_ gameId: String, _ testModeEnabled: Bool) -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                if !UnityAds.isSupported() {
                    self._logger.info("\(kTag): \(#function): not supported")
                    single(.success(false))
                    return
                }
                if UnityAds.isInitialized() {
                    self._initialized = true
                    self._logger.info("\(kTag): \(#function): initialized")
                    single(.success(true))
                    return
                }
                if self._initializing {
                    self._logger.info("\(kTag): \(#function): initializing")
                    single(.success(false))
                    return
                }
                if self._initialized {
                    self._logger.info("\(kTag): \(#function): initialized")
                    single(.success(true))
                    return
                }
                self._initializing = true
                UnityAds.initialize(gameId, testMode: testModeEnabled, initializationDelegate: InitializeDelegate {
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): initializationComplete")
                        self._initializing = false
                        self._initialized = true
                        single(.success(true))
                    }
                } _: { error, message in
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): initializationFailed: error = \(error) message = \(message)")
                        self._initializing = false
                        single(.success(false))
                    }
                 })
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    func setDebugModeEnabled(_ enabled: Bool) {
        Thread.runOnMainThread {
            if !self._initialized {
                return
            }
            UnityAds.setDebugMode(enabled)
        }
    }

    func createBannerAd(_ adId: String, _ adSize: CGSize) -> Bool {
        return createAd(adId) {
            UnityBannerAd(self._bridge, self._logger, adId, adSize, self._bannerHelper)
        }
    }

    func createAd(_ adId: String, _ creator: () -> IAd) -> Bool {
        if _ads.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = creator()
        _ads[adId] = ad
        return true
    }

    func destroyAd(_ adId: String) -> Bool {
        guard let ad = _ads[adId] else {
            return false
        }
        ad.destroy()
        _ads.removeValue(forKey: adId)
        return true
    }

    func hasRewardedAd(_ adId: String) -> Bool {
        if !_initialized {
            return false
        }
        return _loadedAdIds.contains(adId)
    }

    func loadRewardedAd(_ adId: String) -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                self._logger.debug("\(kTag): \(#function): id = \(adId)")
                if !self._initialized {
                    self._logger.debug("\(kTag): \(#function): not initialized")
                    single(.success(false))
                    return
                }
                UnityAds.load(adId, loadDelegate: LoadDelegate { _ in
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): unityAdsAdLoaded: \(adId)")
                        single(.success(true))
                    }
                } _: { _ in
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): unityAdsAdFailed: \(adId)")
                        single(.success(false))
                    }
                 })
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    func showRewardedAd(_ adId: String) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(adId)")
            if !self._initialized {
                self._logger.debug("\(kTag): \(#function): not initialized")
                self._bridge.callCpp(kOnFailedToShow, EEJsonUtils.convertDictionary(toString: [
                    "ad_id": adId,
                    "message": ""
                ]))
                return
            }
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            UnityAds.show(rootView, placementId: adId, showDelegate: ShowDelegate { message in
                Thread.runOnMainThread {
                    self._logger.debug("\(kTag): unityAdsOnFailedToShow: \(adId)")
                    self._bridge.callCpp(kOnFailedToShow, EEJsonUtils.convertDictionary(toString: [
                        "ad_id": adId,
                        "message": message
                    ]))
                }
            } _: { rewarded in
                Thread.runOnMainThread {
                    self._logger.debug("\(kTag): unityAdsOnClosed: \(adId) rewarded = \(rewarded)")
                    self._bridge.callCpp(kOnClosed, EEJsonUtils.convertDictionary(toString: [
                        "ad_id": adId,
                        "rewarded": rewarded
                    ]))
                }
            })
        }
    }

    func unityAdsReady(_ adId: String) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(adId)")
            self._loadedAdIds.insert(adId)
            self._bridge.callCpp(kOnLoaded, adId)
        }
    }

    func unityAdsDidStart(_ adId: String) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(adId)")
        }
    }
}
