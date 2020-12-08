//
//  AdMobInterstitialAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

private let kTag = "\(AdMobInterstitialAd.self)"

internal class AdMobInterstitialAd: NSObject, IInterstitialAd, GADInterstitialDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: InterstitialAdHelper?
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
        _helper = InterstitialAdHelper(_bridge, self, _messageHelper)
        registerHandlers()
        createInternalAd()
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
    
    func createInternalAd() {
        Thread.runOnMainThread {
            if self._ad != nil {
                return
            }
            let ad = GADInterstitial(adUnitID: self._adId)
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
                return
            }
            ad.load(GADRequest())
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            guard
                let ad = self._ad,
                let rootView = Utils.getCurrentRootViewController()
            else {
                assert(false, "Ad is not initialized")
                return
            }
            ad.present(fromRootViewController: rootView)
        }
    }
    
    func interstitialDidReceiveAd(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }
    
    func interstitial(_ ad: GADInterstitial, didFailToReceiveAdWithError error: GADRequestError) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self.destroyInternalAd()
            self.createInternalAd()
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
        }
    }
    
    func interstitialWillPresentScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = false
        }
    }
    
    func interstitialDidFail(toPresentScreen ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self.destroyInternalAd()
            self.createInternalAd()
            self._bridge.callCpp(self._messageHelper.onFailedToShow)
        }
    }
    
    func interstitialWillLeaveApplication(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }
    
    func interstitialWillDismissScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }
    
    func interstitialDidDismissScreen(_ ad: GADInterstitial) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self.destroyInternalAd()
            self.createInternalAd()
            self._bridge.callCpp(self._messageHelper.onClosed)
        }
    }
}
