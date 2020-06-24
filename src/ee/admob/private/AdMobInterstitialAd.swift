//
//  AdMobInterstitialAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

internal class AdMobInterstitialAd: NSObject, IInterstitialAd, GADInterstitialDelegate {
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: InterstitialAdHelper?
    private var _isLoaded = false
    private var _ad: GADInterstitial?
    
    init(_ bridge: IMessageBridge,
         _ adId: String) {
        _bridge = bridge
        _adId = adId
        _messageHelper = MessageHelper("AdMobInterstitialAd", _adId)
        super.init()
        _helper = InterstitialAdHelper(_bridge, self, _messageHelper)
        registerHandlers()
        Thread.runOnMainThread {
            self.createInternalAd()
        }
    }
    
    func destroy() {
        deregisterHandlers()
        Thread.runOnMainThread {
            self.destroyInternalAd()
        }
    }
    
    func registerHandlers() {
        _helper?.registerHandlers()
        _bridge.registerHandler(_messageHelper.createInternalAd) { _ in
            self.createInternalAd()
            return ""
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) { _ in
            self.destroyInternalAd()
            return ""
        }
    }
    
    func deregisterHandlers() {
        _helper?.deregisterHandlers()
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
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
            }
            ad.load(GADRequest())
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
            ad.present(fromRootViewController: rootView)
        }
    }
    
    func interstitialDidReceiveAd(_ ad: GADInterstitial) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func interstitial(_ ad: GADInterstitial, didFailToReceiveAdWithError error: GADRequestError) {
        print("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func interstitialWillPresentScreen(_ ad: GADInterstitial) {
        print("\(#function)")
        _isLoaded = false
    }
    
    func interstitialDidFail(toPresentScreen ad: GADInterstitial) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onFailedToShow)
    }
    
    func interstitialWillDismissScreen(_ ad: GADInterstitial) {
        print("\(#function)")
    }
    
    func interstitialDidDismissScreen(_ ad: GADInterstitial) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onClosed)
    }
    
    func interstitialWillLeaveApplication(_ ad: GADInterstitial) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
}
