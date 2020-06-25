//
//  FacebookNativeAd.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import FBAudienceNetwork
import Foundation

internal class FacebookNativeAd:
    NSObject, IAdView, FBNativeAdDelegate {
    private let _logger = Logger("\(FacebookNativeAd.self)")
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _layoutName: String
    private let _messageHelper: MessageHelper
    private var _helper: AdViewHelper?
    private var _isLoaded = false
    private var _ad: FBNativeAd?
    private var _view: EEFacebookNativeAdView?
    private var _viewHelper: ViewHelper?
    
    init(_ bridge: IMessageBridge,
         _ adId: String,
         _ layoutName: String) {
        _bridge = bridge
        _adId = adId
        _layoutName = layoutName
        _messageHelper = MessageHelper("FacebookNativeAd", _adId)
        super.init()
        _helper = AdViewHelper(_bridge, self, _messageHelper)
        registerHandlers()
        createInternalAd()
        createView()
    }
    
    func destroy() {
        deregisterHandlers()
        destroyView()
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
            self._isLoaded = false
            let ad = FBNativeAd(placementID: self._adId)
            ad.delegate = self
            self._ad = ad
        }
    }
    
    func destroyInternalAd() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                return
            }
            self._isLoaded = false
            ad.delegate = nil
            ad.unregisterView()
            self._ad = nil
        }
    }
    
    func createAdView() -> EEFacebookNativeAdView {
        guard let nibObjects = Bundle(for: EEFacebookNativeAdView.self).loadNibNamed(_layoutName,
                                                                                     owner: nil,
                                                                                     options: nil) else {
            assert(false, "Invalid layout")
        }
        guard let view = nibObjects[0] as? EEFacebookNativeAdView else {
            assert(false, "Invalid layout class")
        }
        return view
    }
    
    func createView() {
        Thread.runOnMainThread {
            assert(self._view == nil)
            let view = self.createAdView()
            view.adchoicesView.corner = UIRectCorner.topRight
            self._view = view
            self._viewHelper = ViewHelper(view)
            let rootView = Utils.getCurrentRootViewController()
            rootView?.view.addSubview(view)
        }
    }
    
    func destroyView() {
        Thread.runOnMainThread {
            guard let view = self._view else {
                return
            }
            view.removeFromSuperview()
            self._view = nil
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
            ad.loadAd(withMediaCachePolicy: FBNativeAdsCachePolicy.all)
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
        set(value) {
            _viewHelper?.isVisible = value
            if value {
                _view?.subviews.forEach { $0.setNeedsDisplay() }
            }
        }
    }
    
    func nativeAdDidLoad(_ ad: FBNativeAd) {
        _logger.debug("\(#function)")
        assert(ad == _ad)
        
        guard let view = _view else {
            assert(false, "Ad view is not initialized")
        }
        
        ad.unregisterView()
        if view.callToActionButton != nil {
            let rootView = Utils.getCurrentRootViewController()
            ad.registerView(forInteraction: view,
                            mediaView: view.mediaView,
                            iconView: view.iconView,
                            viewController: rootView,
                            clickableViews: [view.callToActionButton])
        }
        view.adchoicesView.nativeAd = ad
        view.bodyLabel.text = ad.bodyText
        view.callToActionButton.setTitle(ad.callToAction,
                                         for: UIControl.State.normal)
        view.socialContextLabel.text = ad.socialContext
        view.sponsorLabel.text = ad.sponsoredTranslation
        view.titleLabel.text = ad.headline
        
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }
    
    func nativeAd(_ nativeAd: FBNativeAd, didFailWithError error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }
    
    func nativeAdWillLogImpression(_ nativeAd: FBNativeAd) {
        _logger.debug("\(#function)")
    }
    
    func nativeAdDidClick(_ nativeAd: FBNativeAd) {
        _logger.debug("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }
    
    func nativeAdDidFinishHandlingClick(_ nativeAd: FBNativeAd) {
        _logger.debug("\(#function)")
    }
    
    func nativeAdDidDownloadMedia(_ nativeAd: FBNativeAd) {
        _logger.debug("\(#function)")
    }
}
