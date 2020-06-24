//
//  AdMobBannerAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

internal class AdMobBannerAd: NSObject, IAdView, GADBannerViewDelegate {
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _adSize: GADAdSize
    private let _messageHelper: MessageHelper
    private var _helper: AdViewHelper?
    private var _isLoaded = false
    private var _ad: GADBannerView?
    private var _viewHelper: ViewHelper?

    init(_ bridge: IMessageBridge,
         _ adId: String,
         _ adSize: GADAdSize) {
        _bridge = bridge
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("AdMobBannerAd", _adId)
        super.init()
        _helper = AdViewHelper(_bridge, self, _messageHelper)
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
            if self._ad == nil {
                return
            }
            self._isLoaded = false
            let ad = GADBannerView(adSize: self._adSize)
            ad.adUnitID = self._adId
            ad.delegate = self
            self._ad = ad
            self._viewHelper = ViewHelper(ad)
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
            ad.removeFromSuperview()
            ad.delegate = nil
            self._ad = nil
            self._viewHelper = nil
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

    var position: CGPoint {
        get { return _viewHelper?.position ?? CGPoint.zero }
        set(value) { _viewHelper?.position = value }
    }

    var size: CGSize {
        get { return _viewHelper?.size ?? CGSize.zero }
        set(value) { _viewHelper?.size = value }
    }

    var isVisible: Bool {
        get { return _viewHelper?.isVisible ?? false }
        set(value) { _viewHelper?.isVisible = value }
    }

    func adViewDidReceiveAd(_ bannerView: GADBannerView) {
        print("\(#function)")
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    func adView(_ bannerView: GADBannerView, didFailToReceiveAdWithError error: GADRequestError) {
        print("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }

    func adViewWillPresentScreen(_ bannerView: GADBannerView) {
        print("\(#function)")
    }

    func adViewWillDismissScreen(_ bannerView: GADBannerView) {
        print("\(#function)")
    }

    func adViewDidDismissScreen(_ bannerView: GADBannerView) {
        print("\(#function)")
    }

    func adViewWillLeaveApplication(_ bannerView: GADBannerView) {
        print("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
}
