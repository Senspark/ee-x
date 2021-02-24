//
//  AdMobInterstitialAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import GoogleMobileAds

private let kTag = "\(AdMobInterstitialAd.self)"

internal class AdMobInterstitialAd: NSObject, IFullScreenAd, GADFullScreenContentDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: FullScreenAdHelper?
    private var _isLoaded = false
    private var _ad: GADInterstitialAdBeta?
    
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
    }
    
    func registerHandlers() {
        _helper?.registerHandlers()
    }
    
    func deregisterHandlers() {
        _helper?.deregisterHandlers()
    }
    
    var isLoaded: Bool {
        return _isLoaded
    }
    
    func load() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            GADInterstitialAdBeta.load(withAdUnitID: self._adId, request: GADRequest()) { ad, error in
                if let error = error {
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): \(#function): failed id = \(self._adId) message = \(error.localizedDescription)")
                        self._bridge.callCpp(self._messageHelper.onFailedToLoad, EEJsonUtils.convertDictionary(toString: [
                            "code": (error as NSError).code,
                            "message": error.localizedDescription
                        ]))
                    }
                } else {
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): \(#function): succeeded id = \(self._adId)")
                        self._isLoaded = true
                        self._ad = ad
                        self._ad?.fullScreenContentDelegate = self
                        self._bridge.callCpp(self._messageHelper.onLoaded)
                    }
                }
            }
        }
    }
    
    func show() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            guard let ad = self._ad else {
                self._bridge.callCpp(self._messageHelper.onFailedToShow, EEJsonUtils.convertDictionary(toString: [
                    "code": -1,
                    "message": "Null ad"
                ]))
                return
            }
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Current rootView is null")
                self._bridge.callCpp(self._messageHelper.onFailedToShow, EEJsonUtils.convertDictionary(toString: [
                    "code": -1,
                    "message": "Null root view"
                ]))
                return
            }
            ad.present(fromRootViewController: rootView)
        }
    }
    
    func adDidPresentFullScreenContent(_ ad: GADFullScreenPresentingAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
    
    func ad(_ ad: GADFullScreenPresentingAd, didFailToPresentFullScreenContentWithError error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._isLoaded = false
            self._ad = nil
            self._bridge.callCpp(self._messageHelper.onFailedToShow, EEJsonUtils.convertDictionary(toString: [
                "code": (error as NSError).code,
                "message": error.localizedDescription
            ]))
        }
    }
    
    func adDidDismissFullScreenContent(_ ad: GADFullScreenPresentingAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = false
            self._ad = nil
            self._bridge.callCpp(self._messageHelper.onClosed)
        }
    }
}
