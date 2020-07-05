//
//  AdMobNativeAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

/// https://stackoverflow.com/a/24592029
func dictionaryOfNames(_ arr: UIView...) -> [String: UIView] {
    var d = [String: UIView]()
    for (ix, v) in arr.enumerated() {
        d["v\(ix + 1)"] = v
    }
    return d
}

internal class AdMobNativeAd: NSObject, IAdView,
    GADUnifiedNativeAdLoaderDelegate,
    GADUnifiedNativeAdDelegate {
    private let _logger = Logger("\(AdMobNativeAd.self)")
    private let _bridge: IMessageBridge
    private let _adId: String
    private let _layoutName: String
    private let _messageHelper: MessageHelper
    private var _helper: AdViewHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: GADAdLoader?
    private var _view: GADUnifiedNativeAdView?

    init(_ bridge: IMessageBridge,
         _ adId: String,
         _ layoutName: String) {
        _bridge = bridge
        _adId = adId
        _layoutName = layoutName
        _messageHelper = MessageHelper("AdMobNativeAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, CGSize.zero, false)
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
            let options = GADVideoOptions()
            options.startMuted = true
            let rootView = Utils.getCurrentRootViewController()
            let ad = GADAdLoader(adUnitID: self._adId,
                                 rootViewController: rootView,
                                 adTypes: [GADAdLoaderAdType.unifiedNative],
                                 options: [options])
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
            self._ad = nil
        }
    }

    func createAdView() -> GADUnifiedNativeAdView {
        guard let nibObjects = Bundle(for: GADUnifiedNativeAdView.self).loadNibNamed(_layoutName,
                                                                                     owner: nil,
                                                                                     options: nil) else {
            assert(false, "Invalid layout")
        }
        guard let view = nibObjects[0] as? GADUnifiedNativeAdView else {
            assert(false, "Invalid layout class")
        }
        return view
    }

    func createView() {
        Thread.runOnMainThread {
            assert(self._view == nil)
            let view = self.createAdView()
            self._view = view
            self._viewHelper.view = view
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
            ad.load(GADRequest())
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
        set(value) {
            _viewHelper.isVisible = value
            if value {
                Thread.runOnMainThread {
                    self._view?.subviews.forEach { $0.setNeedsDisplay() }
                }
            }
        }
    }

    /// Gets an image representing the number of stars. Returns nil if rating is
    /// less than 3.5 stars.
    func imageForStars(_ stars: Float) -> UIImage? {
        switch stars {
            case _ where stars >= 5: return UIImage(named: "stars_5")
            case _ where stars >= 4.5: return UIImage(named: "stars_4_5")
            case _ where stars >= 4: return UIImage(named: "stars_4")
            case _ where stars >= 3.5: return UIImage(named: "stars_3_5")
            default: return nil
        }
    }

    func adLoader(_ adLoader: GADAdLoader, didReceive ad: GADUnifiedNativeAd) {
        guard let view = _view else {
            assert(false, "Ad view is not initialized")
        }

        // Populate the native ad view with the native ad assets.
        // The headline and mediaContent are guaranteed to be present in every
        // native ad.
        (view.headlineView as? UILabel)?.text = ad.headline
        view.mediaView?.mediaContent = ad.mediaContent

        // These assets are not guaranteed to be present. Check that they are before
        // showing or hiding them.
        (view.bodyView as? UILabel)?.text = ad.body
        view.bodyView?.isHidden = ad.body == nil

        (view.callToActionView as? UIButton)?.setTitle(ad.callToAction, for: UIControl.State.normal)
        view.callToActionView?.isHidden = ad.callToAction == nil

        (view.iconView as? UIImageView)?.image = ad.icon?.image
        view.iconView?.isHidden = ad.icon == nil

        (view.starRatingView as? UIImageView)?.image = imageForStars(ad.starRating?.floatValue ?? 0)
        view.starRatingView?.isHidden = ad.starRating == nil

        (view.storeView as? UILabel)?.text = ad.store
        view.storeView?.isHidden = ad.store == nil

        (view.priceView as? UILabel)?.text = ad.price
        view.priceView?.isHidden = ad.price == nil

        (view.advertiserView as? UILabel)?.text = ad.advertiser
        view.advertiserView?.isHidden = ad.advertiser == nil

        // In order for the SDK to process touch events properly, user interaction
        // should be disabled.
        view.callToActionView?.isUserInteractionEnabled = false

        // Set ourselves as the ad delegate to be notified of native ad events.
        ad.delegate = self

        view.nativeAd = ad

        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    func adLoader(_ adLoader: GADAdLoader, didFailToReceiveAdWithError error: GADRequestError) {
        _logger.debug("\(#function): \(error.localizedDescription)")
        _bridge.callCpp(_messageHelper.onFailedToLoad, error.localizedDescription)
    }

    func adLoaderDidFinishLoading(_ adLoader: GADAdLoader) {
        _logger.debug("\(#function)")
    }

    func nativeAdWillPresentScreen(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
    }

    func nativeAdDidRecordImpression(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
    }

    func nativeAdDidRecordClick(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
    }

    func nativeAdWillLeaveApplication(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
        _bridge.callCpp(_messageHelper.onClicked)
    }

    func nativeAdWillDismissScreen(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
    }

    func nativeAdDidDismissScreen(_ nativeAd: GADUnifiedNativeAd) {
        _logger.debug("\(#function)")
    }
}
