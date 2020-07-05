//
//  FacebookBannerAd.swift
//  ee-x-6914a733
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork

internal class FacebookBannerAd:
    NSObject, IAdView, FBAdViewDelegate {
    private let _logger = Logger("\(FacebookBannerAd.self)")
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _adSize: FBAdSize
    private let _messageHelper: MessageHelper
    private var _helper: AdViewHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: FBAdView?
    
    init(_ bridge: IMessageBridge,
         _ adId: String,
         _ adSize: FBAdSize,
         _ bannerHelper: FacebookBannerHelper) {
        _bridge = bridge
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("FacebookBannerAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, bannerHelper.getSize(adSize: adSize), false)
        super.init()
        _helper = AdViewHelper(_bridge, self, _messageHelper)
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
            guard let ad = self._ad else {
                assert(false, "Ad is not initialized")
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
        _logger.debug("\(#function)")
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func adView(_ adView: FBAdView, didFailWithError error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func adViewDidClick(_ adView: FBAdView) {
        _logger.debug("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
    
    func adViewDidFinishHandlingClick(_ adView: FBAdView) {
        _logger.debug("\(#function)")
    }
    
    func adViewWillLogImpression(_ adView: FBAdView) {
        _logger.debug("\(#function)")
    }
}
