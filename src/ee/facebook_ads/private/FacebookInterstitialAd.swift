//
//  FacebookInterstitialAd.swift
//  Pods
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork
import Foundation

internal class FacebookInterstitialAd:
    NSObject, IInterstitialAd, FBInterstitialAdDelegate {
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: InterstitialAdHelper?
    private var _isLoaded = false
    private var _ad: FBInterstitialAd?
    
    init(_ bridge: IMessageBridge,
         _ adId: String) {
        _bridge = bridge
        _adId = adId
        _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
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
            let ad = FBInterstitialAd(placementID: self._adId)
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
            let result = ad.show(fromRootViewController: rootView)
            if result {
                // OK.
            } else {
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
            }
        }
    }
    
    func interstitialAdDidLoad(_ interstitialAd: FBInterstitialAd) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func interstitialAd(_ interstitialAd: FBInterstitialAd, didFailWithError error: Error) {
        print("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func interstitialAdWillLogImpression(_ interstitialAd: FBInterstitialAd) {
        print("\(#function)")
    }
    
    func interstitialAdDidClick(_ interstitialAd: FBInterstitialAd) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
    
    func interstitialAdWillClose(_ interstitialAd: FBInterstitialAd) {
        print("\(#function)")
    }
    
    func interstitialAdDidClose(_ interstitialAd: FBInterstitialAd) {
        print("\(#function)")
        _isLoaded = false
        _bridge.callCpp(_messageHelper.onClosed)
    }
}
