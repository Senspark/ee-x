//
//  AdMobBannerAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import GoogleMobileAds

private let kTag = "\(AdMobBannerAd.self)"

internal class AdMobBannerAd: NSObject, IBannerAd, GADBannerViewDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _adSize: GADAdSize
    private let _messageHelper: MessageHelper
    private var _helper: BannerAdHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: GADBannerView?

    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String,
         _ adSize: GADAdSize,
         _ bannerHelper: AdMobBannerHelper) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("AdMobBannerAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, bannerHelper.getSize(adSize: adSize), false)
        super.init()
        _helper = BannerAdHelper(_bridge, self, _messageHelper)
        registerHandlers()
        createInternalAd()
    }

    func destroy() {
        deregisterHandler()
        destroyInternalAd()
    }

    func registerHandlers() {
        _helper?.registerHandlers()
    }

    func deregisterHandler() {
        _helper?.deregisterHandlers()
    }

    func createInternalAd() {
        Thread.runOnMainThread {
            if self._ad != nil {
                return
            }
            self._isLoaded = false
            let ad = GADBannerView(adSize: self._adSize)
            ad.adUnitID = self._adId
            ad.delegate = self
            self._ad = ad
            self._viewHelper.view = ad
            let rootView = Utils.getCurrentRootViewController()
            ad.rootViewController = rootView
            rootView?.view.addSubview(ad)
        }
    }

    func destroyInternalAd() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                return
            }
            self._isLoaded = false
            ad.delegate = nil
            ad.removeFromSuperview()
            self._ad = nil
            self._viewHelper.view = nil
        }
    }

    var isLoaded: Bool {
        return _isLoaded
    }

    func load() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            guard let ad = self._ad else {
                assert(false, "Ad is not initialized")
                return
            }
            ad.load(GADRequest())
        }
    }

    var isVisible: Bool {
        get { return _viewHelper.isVisible }
        set(value) { _viewHelper.isVisible = value }
    }

    var position: CGPoint {
        get { return _viewHelper.position }
        set(value) { _viewHelper.position = value }
    }

    var size: CGSize {
        get { return _viewHelper.size }
        set(value) { _viewHelper.size = value }
    }

    func bannerViewDidReceiveAd(_ bannerView: GADBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }

    func bannerView(_ bannerView: GADBannerView, didFailToReceiveAdWithError error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, EEJsonUtils.convertDictionary(toString: [
                "code": (error as NSError).code,
                "message": error.localizedDescription
            ]))
        }
    }

    func bannerViewDidRecordImpression(_ bannerView: GADBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }

    func bannerViewWillPresentScreen(_ bannerView: GADBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }

    func bannerViewWillDismissScreen(_ bannerView: GADBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }

    func bannerViewDidDismissScreen(_ bannerView: GADBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
}
