//
//  Vungle.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import Foundation

private let kPrefix = "VungleBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kHasRewardedAd = "\(kPrefix)HasRewardedAd"
private let kLoadRewardedAd = "\(kPrefix)LoadRewardedAd"
private let kShowRewardedAd = "\(kPrefix)ShowRewardedAd"
private let kOnLoaded = "\(kPrefix)OnLoaded"
private let kOnFailedToLoad = "\(kPrefix)OnFailedToLoad"
private let kOnFailedToShow = "\(kPrefix)OnFailedToShow"
private let kOnClicked = "\(kPrefix)OnClicked"
private let kOnClosed = "\(kPrefix)OnClosed"

@objc(EEVungleBridge)
public class VungleBridge: NSObject, IPlugin, VungleSDKDelegate {
    private let _logger = Logger("\(VungleBridge.self)")
    private let _bridge: IMessageBridge
    private let _sdk = VungleSDK.shared()
    private var _initializing = false
    private var _initialized = false
    private var _rewarded = false
    private var _loadingAdIds: Set<String> = []
    private var _loadedAdIds: Set<String> = []

    public required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        Thread.runOnMainThread {
            if !self._initialized {
                return
            }
            self._sdk.delegate = nil
        }
    }

    func registerHandlers() {}

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    func initialize(_ applicationId: String) {
        Thread.runOnMainThread {
            if self._initializing {
                return
            }
            if self._initialized {
                return
            }
            self._initializing = true
            self._sdk.delegate = self
            do {
                try self._sdk.start(withAppId: applicationId)
            } catch {
                self._logger.debug("\(#function): \(error.localizedDescription)")
                return
            }
        }
    }

    func hasRewardedAd(_ adId: String) -> Bool {
        return _loadedAdIds.contains(adId)
    }

    func loadRewardedAd(_ adId: String) {
        Thread.runOnMainThread {
            do {
                self._loadingAdIds.insert(adId)
                try self._sdk.loadPlacement(withID: adId)
            } catch {
                self._loadingAdIds.remove(adId)
                self._bridge.callCpp(kOnFailedToLoad, EEJsonUtils.convertDictionary(toString: [
                    "ad_id": adId,
                    "message": error.localizedDescription
                ]))
                return
            }
            // OK.
        }
    }

    func showRewardedAd(_ adId: String) {
        Thread.runOnMainThread {
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
            }
            self._rewarded = false
            do {
                try self._sdk.playAd(rootView, options: nil, placementID: adId)
            } catch {
                self._bridge.callCpp(kOnFailedToShow, EEJsonUtils.convertDictionary(toString: [
                    "ad_id": adId,
                    "message": error.localizedDescription
                ]))
                return
            }
            // OK.
        }
    }

    public func vungleSDKDidInitialize() {
        _logger.debug("\(#function)")
        _initializing = false
        _initialized = false
    }

    public func vungleSDKFailedToInitializeWithError(_ error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
        _initializing = false
    }

    public func vungleAdPlayabilityUpdate(_ isAdPlayable: Bool, adId: String?, error: Error?) {
        _logger.debug("\(#function): playable = \(isAdPlayable) id = \(adId ?? "") reason \(error?.localizedDescription ?? "")")
        if let adId = adId {
            if isAdPlayable {
                _loadedAdIds.insert(adId)
            }
            if _loadingAdIds.contains(adId) {
                assert(isAdPlayable)
                _loadingAdIds.remove(adId)
                _bridge.callCpp(kOnLoaded, EEJsonUtils.convertDictionary(toString: [
                    "ad_id": adId
                ]))
                return
            }
        }
        if error != nil {
            assert(adId == nil)
            return
        }
        // Mediation ???
    }

    public func vungleWillShowAd(forPlacementID adId: String?) {
        _logger.debug("\(#function): \(adId ?? "")")
    }

    public func vungleDidShowAd(forPlacementID adId: String?) {
        _logger.debug("\(#function): \(adId ?? "")")
    }

    public func vungleTrackClick(forPlacementID adId: String?) {
        _logger.debug("\(#function): \(adId ?? "")")
        _bridge.callCpp(kOnClicked, adId ?? "")
    }

    public func vungleWillLeaveApplication(forPlacementID adId: String?) {
        _logger.debug("\(#function): \(adId ?? "")")
    }

    public func vungleRewardUser(forPlacementID adId: String?) {
        _logger.debug("\(#function): \(adId ?? "")")
        _rewarded = true
    }

    public func vungleWillCloseAd(forPlacementID adId: String) {
        _logger.debug("\(#function): \(adId)")
    }

    public func vungleDidCloseAd(forPlacementID adId: String) {
        _logger.debug("\(#function): \(adId)")
        _loadedAdIds.remove(adId)
        _bridge.callCpp(kOnClosed, EEJsonUtils.convertDictionary(toString: [
            "ad_id": adId,
            "rewarded": _rewarded
        ]))
    }
}
