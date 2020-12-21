//
//  FacebookInterstitialAd.swift
//  Pods
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork
import Foundation

private let kTag = "\(FacebookInterstitialAd.self)"

internal class FacebookInterstitialAd:
    NSObject, IInterstitialAd, FBInterstitialAdDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: InterstitialAdHelper?
    private var _isLoaded = false
    private var _displaying = false
    private var _ad: FBInterstitialAd?
    
    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
        super.init()
        _helper = InterstitialAdHelper(_bridge, self, _messageHelper)
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
    
    func createInternalAd() -> FBInterstitialAd {
        if let currentAd = _ad {
            return currentAd
        }
        let ad = FBInterstitialAd(placementID: _adId)
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
            let ad = self.createInternalAd()
            ad.load()
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Current rootView is null")
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
                return
            }
            self._displaying = true
            let ad = self.createInternalAd()
            let result = ad.show(fromRootViewController: rootView)
            if result {
                // OK.
                self._isLoaded = false
            } else {
                self.destroyInternalAd()
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
            }
        }
    }
    
    func interstitialAdDidLoad(_ interstitialAd: FBInterstitialAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }
    
    func interstitialAd(_ interstitialAd: FBInterstitialAd, didFailWithError error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self.destroyInternalAd()
            if self._displaying {
                self._displaying = false
                self._bridge.callCpp(self._messageHelper.onFailedToShow, error.localizedDescription)
            } else {
                self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
            }
        }
    }
    
    func interstitialAdWillLogImpression(_ interstitialAd: FBInterstitialAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }
    
    func interstitialAdDidClick(_ interstitialAd: FBInterstitialAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }
    
    func interstitialAdWillClose(_ interstitialAd: FBInterstitialAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }
    
    func interstitialAdDidClose(_ interstitialAd: FBInterstitialAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._displaying = false
            self.destroyInternalAd()
            self._bridge.callCpp(self._messageHelper.onClosed)
        }
    }
}
