//
//  AdMobRewardedAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

internal class AdMobRewardedAd: NSObject, GADRewardedAdDelegate {
    private let _logger = Logger("\(AdMobRewardedAd.self)")
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _isLoaded = false
    private var _rewarded = false
    private var _ad: GADRewardedAd?
    
    init(_ bridge: IMessageBridge,
         _ adId: String) {
        _bridge = bridge
        _adId = adId
        _messageHelper = MessageHelper("AdMobRewardedAd", _adId)
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
            self._ad = GADRewardedAd(adUnitID: self._adId)
        }
    }
    
    func destroyInternalAd() {
        Thread.runOnMainThread {
            if self._ad == nil {
                return
            }
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
            ad.load(GADRequest(), completionHandler: { error in
                if let error = error {
                    self._bridge.callCpp(self._messageHelper.onFailedToLoad,
                                         error.localizedDescription)
                } else {
                    self._bridge.callCpp(self._messageHelper.onLoaded)
                }
            })
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
            ad.present(fromRootViewController: rootView, delegate: self)
        }
    }
    
    func rewardedAdDidPresent(_ rewardedAd: GADRewardedAd) {
        _logger.debug("\(#function)")
    }
    
    func rewardedAd(_ rewardedAd: GADRewardedAd, didFailToPresentWithError error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToShow, error.localizedDescription)
    }
    
    func rewardedAd(_ rewardedAd: GADRewardedAd, userDidEarn reward: GADAdReward) {
        _logger.debug("\(#function)")
        _rewarded = true
    }
    
    func rewardedAdDidDismiss(_ rewardedAd: GADRewardedAd) {
        _logger.debug("\(#function)")
        _isLoaded = false
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}
