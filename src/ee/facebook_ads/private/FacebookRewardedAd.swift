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
            ad.load()
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                assert(false, "Ad is not initialized")
            }
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
            }
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
        print("\(#function)")
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func rewardedVideoAd(_ rewardedVideoAd: FBRewardedVideoAd, didFailWithError error: Error) {
        print("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func rewardedVideoAdDidClick(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
    
    func rewardedVideoAdWillLogImpression(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
    }
    
    func rewardedVideoAdVideoComplete(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
        _rewarded = true
    }
    
    func rewardedVideoAdServerRewardDidSucceed(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
    }
    
    func rewardedVideoAdServerRewardDidFail(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
    }
    
    func rewardedVideoAdWillClose(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
    }
    
    func rewardedVideoAdDidClose(_ rewardedVideoAd: FBRewardedVideoAd) {
        print("\(#function)")
        _isLoaded = false
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}
