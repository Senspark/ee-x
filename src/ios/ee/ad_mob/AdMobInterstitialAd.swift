//
//  AdMobInterstitialAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

private let kTag = "\(AdMobInterstitialAd.self)"

internal class AdMobInterstitialAd: NSObject, IFullScreenAd, GADInterstitialDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: FullScreenAdHelper?
    private var _isLoaded = false
    private var _ad: GADInterstitial?
    
    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _messageHelper = MessageHelper("AdMobInterstitialAd", _adId)
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
    
    func createInternalAd() -> GADInterstitial {
        if let currentAd = _ad {
            return currentAd
        }
        let ad = GADInterstitial(adUnitID: _adId)
        ad.delegate = self
        _ad = ad
        return ad
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
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            let ad = self.createInternalAd()
            ad.load(GADRequest())
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
            let ad = self.createInternalAd()
            ad.present(fromRootViewController: rootView)
        }
    }
    
    func interstitialDidReceiveAd(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }
    
    func interstitial(_ ad: GADInterstitial, didFailToReceiveAdWithError error: GADRequestError) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
        }
    }
    
    func interstitialWillPresentScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = false
        }
    }
    
    func interstitialDidFail(toPresentScreen ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onFailedToShow)
        }
    }
    
    func interstitialWillLeaveApplication(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }
    
    func interstitialWillDismissScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
    
    func interstitialDidDismissScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onClosed)
        }
    }
}
