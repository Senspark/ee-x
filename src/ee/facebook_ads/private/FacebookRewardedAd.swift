//
//  FacebookRewardedAd.swift
//  Pods
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork
import Foundation

internal class FacebookRewardedAd:
    NSObject, FBRewardedVideoAdDelegate {
    private let _logger = Logger("\(FacebookRewardedAd.self)")
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _isLoaded = false
    private var _rewarded = false
    private var _ad: FBRewardedVideoAd?
    
    init(_ bridge: IMessageBridge,
         _ adId: String) {
        _bridge = bridge
        _adId = adId
        _messageHelper = MessageHelper("FacebookRewardedAd", _adId)
        super.init()
        registerHandlers()
        createInternalAd()
    }
    
    func destroy() {
        deregisterHandlers()
        destroyInternalAd()
    }
    
    func registerHandlers() {
        _bridge.registerHandler(_messageHelper.createInternalAd) { _ in
            self.createInternalAd()
            return ""
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) { _ in
            self.destroyInternalAd()
            return ""
        }
        _bridge.registerHandler(_messageHelper.isLoaded) { _ in
            Utils.toString(self.isLoaded)
        }
        _bridge.registerHandler(_messageHelper.load) { _ in
            self.load()
            return ""
        }
        _bridge.registerHandler(_messageHelper.show) { _ in
            self.show()
            return ""
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
        _bridge.deregisterHandler(_messageHelper.isLoaded)
        _bridge.deregisterHandler(_messageHelper.load)
        _bridge.deregisterHandler(_messageHelper.show)
    }
    
    func createInternalAd() {
        Thread.runOnMainThread {
            if self._ad != nil {
                return
            }
            let ad = FBRewardedVideoAd(placementID: self._adId)
            ad.delegate = self
            self._ad = ad
        }
    }
    
    func destroyInternalAd() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                return
            }
            ad.delegate = nil
            self._ad = nil
        }
    }
    
    var isLoaded: Bool {
        return _isLoaded
    }
    
    func load() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                assert(false, "Ad is not initialized")
            }
            self._logger.debug("\(#function): id = \(self._adId)")
            ad.load()
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            guard
                let ad = self._ad,
                let rootView = Utils.getCurrentRootViewController()
            else {
                assert(false, "Ad is not initialized")
            }
            self._logger.debug("\(#function): id = \(self._adId)")
            self._rewarded = false
            let result = ad.show(fromRootViewController: rootView)
            if result {
                // OK.
            } else {
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
            }
        }
    }
    
    func rewardedVideoAdDidLoad(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func rewardedVideoAd(_ rewardedVideoAd: FBRewardedVideoAd, didFailWithError error: Error) {
        _logger.debug("\(#function): id = \(_adId) error = \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func rewardedVideoAdDidClick(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
    
    func rewardedVideoAdWillLogImpression(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
    }
    
    func rewardedVideoAdVideoComplete(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
        _rewarded = true
    }
    
    func rewardedVideoAdServerRewardDidSucceed(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
    }
    
    func rewardedVideoAdServerRewardDidFail(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
    }
    
    func rewardedVideoAdWillClose(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
    }
    
    func rewardedVideoAdDidClose(_ rewardedVideoAd: FBRewardedVideoAd) {
        _logger.debug("\(#function): id = \(_adId)")
        _isLoaded = false
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}
