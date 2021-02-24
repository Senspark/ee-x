//
//  AdMobRewardedInterstitialAd.swift
//  ee-x-4abf52fd
//
//  Created by eps on 1/29/21.
//

import GoogleMobileAds

private let kTag = "\(AdMobRewardedInterstitialAd.self)"

internal class AdMobRewardedInterstitialAd: NSObject, IFullScreenAd, GADFullScreenContentDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _messageHelper: MessageHelper
    private var _helper: FullScreenAdHelper?
    private var _isLoaded = false
    private var _ad: GADRewardedInterstitialAd?
    private var _rewarded = false

    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _messageHelper = MessageHelper("AdMobRewardedInterstitialAd", _adId)
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
            GADRewardedInterstitialAd.load(withAdUnitID: self._adId, request: GADRequest()) { ad, error in
                if error == nil {
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): \(#function): succeeded id = \(self._adId)")
                        self._isLoaded = true
                        self._ad = ad
                        self._ad?.fullScreenContentDelegate = self
                        self._bridge.callCpp(self._messageHelper.onLoaded)
                    }
                } else {
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): \(#function): failed id = \(self._adId) message = \(error?.localizedDescription ?? "")")
                        self._bridge.callCpp(self._messageHelper.onFailedToLoad, error?.localizedDescription ?? "")
                    }
                }
            }
        }
    }

    func show() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            guard let ad = self._ad else {
                assert(false, "Ad is not initialized")
                self._bridge.callCpp(self._messageHelper.onFailedToShow, "Null ad")
                return
            }
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Current rootView is null")
                self._bridge.callCpp(self._messageHelper.onFailedToShow)
                return
            }
            self._rewarded = false
            ad.present(fromRootViewController: rootView) {
                Thread.runOnMainThread {
                    self._rewarded = true
                }
            }
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
            self._bridge.callCpp(self._messageHelper.onFailedToShow, error.localizedDescription)
        }
    }

    func adDidDismissFullScreenContent(_ ad: GADFullScreenPresentingAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = false
            self._ad = nil
            self._bridge.callCpp(self._messageHelper.onClosed, Utils.toString(self._rewarded))
        }
    }
}
