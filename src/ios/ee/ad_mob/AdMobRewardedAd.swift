//
//  AdMobRewardedAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

private let kTag = "\(AdMobRewardedAd.self)"

internal class AdMobRewardedAd: NSObject, IFullScreenAd, GADRewardedAdDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: FullScreenAdHelper?
    private var _isLoaded = false
    private var _rewarded = false
    private var _ad: GADRewardedAd?
    
    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _messageHelper = MessageHelper("AdMobRewardedAd", _adId)
        super.init()
        _helper = FullScreenAdHelper(_bridge, self, _messageHelper)
        registerHandlers()
    }
    
    func destroy() {
        deregisterHandlers()
        destroyInternalAd()
    }
    
    func registerHandlers() {
        _helper?.registerHandlers()
    }
    
    func deregisterHandlers() {
        _helper?.deregisterHandlers()
    }
    
    func createInternalAd() -> GADRewardedAd {
        if let currentAd = _ad {
            return currentAd
        }
        let ad = GADRewardedAd(adUnitID: _adId)
        _ad = ad
        return ad
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
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            let ad = self.createInternalAd()
            ad.load(GADRequest(), completionHandler: { error in
                Thread.runOnMainThread {
                    if let error = error {
                        self._logger.debug("\(kTag): onRewardedAdFailedToLoad: id = \(self._adId) message = \(error.localizedDescription)")
                        self.destroyInternalAd()
                        self._bridge.callCpp(self._messageHelper.onFailedToLoad,
                                             error.localizedDescription)
                    } else {
                        self._logger.debug("\(kTag): onRewardedAdLoaded: id = \(self._adId)")
                        self._isLoaded = true
                        self._bridge.callCpp(self._messageHelper.onLoaded)
                    }
                }
            })
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Current rootView is null")
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
                return
            }
            self._rewarded = false
            let ad = self.createInternalAd()
            ad.present(fromRootViewController: rootView, delegate: self)
        }
    }
    
    func rewardedAdDidPresent(_ rewardedAd: GADRewardedAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
    
    func rewardedAd(_ rewardedAd: GADRewardedAd, didFailToPresentWithError error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._isLoaded = false
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onFailedToShow, error.localizedDescription)
        }
    }
    
    func rewardedAd(_ rewardedAd: GADRewardedAd, userDidEarn reward: GADAdReward) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._rewarded = true
        }
    }
    
    func rewardedAdDidDismiss(_ rewardedAd: GADRewardedAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = false
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onClosed, Utils.toString(self._rewarded))
        }
    }
}
