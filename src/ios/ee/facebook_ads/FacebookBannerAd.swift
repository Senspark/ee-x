//
//  FacebookBannerAd.swift
//  ee-x-6914a733
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork

private let kTag = "\(FacebookBannerAd.self)"

internal class FacebookBannerAd:
    NSObject, IBannerAd, FBAdViewDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _adSize: FBAdSize
    private let _messageHelper: MessageHelper
    private var _helper: BannerAdHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: FBAdView?
    
    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String,
         _ adSize: FBAdSize,
         _ bannerHelper: FacebookBannerHelper) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("FacebookBannerAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, bannerHelper.getSize(adSize: adSize), false)
        super.init()
        _helper = BannerAdHelper(_bridge, self, _messageHelper)
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
            self._isLoaded = false
            let rootView = Utils.getCurrentRootViewController()
            let ad = FBAdView(placementID: self._adId,
                              adSize: self._adSize,
                              rootViewController: rootView)
            ad.delegate = self
            self._ad = ad
            self._viewHelper.view = ad
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
            ad.loadAd()
        }
    }
    
    var position: CGPoint {
        get { return _viewHelper.position }
        set(value) { _viewHelper.position = value }
    }
    
    var size: CGSize {
        get { return _viewHelper.size }
        set(value) { _viewHelper.size = value }
    }
    
    var isVisible: Bool {
        get { return _viewHelper.isVisible }
        set(value) { _viewHelper.isVisible = value }
    }
    
    func adViewDidLoad(_ adView: FBAdView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }
    
    func adView(_ adView: FBAdView, didFailWithError error: Error) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
        }
    }
    
    func adViewDidClick(_ adView: FBAdView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }
    
    func adViewDidFinishHandlingClick(_ adView: FBAdView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
    
    func adViewWillLogImpression(_ adView: FBAdView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
}
