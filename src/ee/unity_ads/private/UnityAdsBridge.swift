//
//  UnityAds.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/25/20.
//

import UnityAds

private let kTag = "\(UnityAdsBridge.self)"
private let kPrefix = "UnityAdsBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetDebugModeEnabled = "\(kPrefix)SetDebugModeEnabled"
private let kHasRewardedAd = "\(kPrefix)HasRewardedAd"
private let kShowRewardedAd = "\(kPrefix)ShowRewardedAd"
private let kOnLoaded = "\(kPrefix)OnLoaded"
private let kOnFailedToShow = "\(kPrefix)OnFailedToShow"
private let kOnClosed = "\(kPrefix)OnClosed"

@objc(EEUnityAdsBridge)
class UnityAdsBridge: NSObject, IPlugin, UnityAdsDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initialized = false
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
        Thread.runOnMainThread {
            if !self._initialized {
                return
            }
            UnityAds.remove(self)
        }
    }

    func registerHandlers() {
        _bridge.registerHandler(kInitialize) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let gameId = dict["gameId"] as? String,
                let testModeEnabled = dict["testModeEnabled"] as? Bool
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.initialize(gameId, testModeEnabled)
            return ""
        }
        _bridge.registerHandler(kSetDebugModeEnabled) { message in
            self.setDebugModeEnabled(Utils.toBool(message))
            return ""
        }
        _bridge.registerHandler(kHasRewardedAd) { message in
            Utils.toString(self.hasRewardedAd(message))
        }
        _bridge.registerHandler(kShowRewardedAd) { message in
            self.showRewardedAd(message)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetDebugModeEnabled)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    func initialize(_ gameId: String, _ testModeEnabled: Bool) {
        Thread.runOnMainThread {
            if self._initialized {
                return
            }
            if !UnityAds.isSupported() {
                return
            }
            UnityAds.initialize(gameId, testMode: testModeEnabled)
            UnityAds.add(self)
            self._initialized = true
        }
    }

    func setDebugModeEnabled(_ enabled: Bool) {
        Thread.runOnMainThread {
            if !self._initialized {
                assert(false, "Please call initialize() first")
            }
            UnityAds.setDebugMode(enabled)
        }
    }

    func hasRewardedAd(_ adId: String) -> Bool {
        return _loadedAdIds.contains(adId)
    }

    func showRewardedAd(_ adId: String) {
        Thread.runOnMainThread {
            if !self._initialized {
                assert(false, "Please call initialize() first")
            }
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            UnityAds.show(rootView, placementId: adId)
        }
    }

    func unityAdsReady(_ adId: String) {
        _logger.debug("\(kTag): \(#function): \(adId)")
        _loadedAdIds.insert(adId)
        _bridge.callCpp(kOnLoaded, adId)
    }

    func unityAdsDidStart(_ adId: String) {
        _logger.debug("\(kTag): \(#function): \(adId)")
    }

    func unityAdsDidFinish(_ adId: String, with state: UnityAdsFinishState) {
        _logger.debug("\(kTag): \(#function): \(adId) state = \(state)")
        if state == UnityAdsFinishState.error {
            _bridge.callCpp(kOnFailedToShow, EEJsonUtils.convertDictionary(toString: [
                "ad_id": adId,
                "message": ""
            ]))
            return
        }
        if state == UnityAdsFinishState.skipped {
            _bridge.callCpp(kOnClosed, EEJsonUtils.convertDictionary(toString: [
                "ad_id": adId,
                "rewarded": false
            ]))
            return
        }
        if state == UnityAdsFinishState.completed {
            _bridge.callCpp(kOnClosed, EEJsonUtils.convertDictionary(toString: [
                "ad_id": adId,
                "rewarded": true
            ]))
            return
        }
        assert(false)
    }

    func unityAdsDidError(_ error: UnityAdsError, withMessage message: String) {
        _logger.debug("\(kTag): \(#function): error = \(error) message = \(message)")
    }
}
