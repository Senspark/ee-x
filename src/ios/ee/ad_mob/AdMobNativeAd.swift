//
//  AdMobNativeAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

/*
  FIXME.
 
private let kTag = "\(AdMobNativeAd.self)"

/// https://stackoverflow.com/a/24592029
func dictionaryOfNames(_ arr: UIView...) -> [String: UIView] {
    var d = [String: UIView]()
    for (ix, v) in arr.enumerated() {
        d["v\(ix + 1)"] = v
    }
    return d
}

internal class AdMobNativeAd: NSObject, IBannerAd,
    GADUnifiedNativeAdLoaderDelegate,
    GADUnifiedNativeAdDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _layoutName: String
    private let _messageHelper: MessageHelper
    private var _helper: BannerAdHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: GADAdLoader?
    private var _view: GADUnifiedNativeAdView?

    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String,
         _ layoutName: String) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _layoutName = layoutName
        _messageHelper = MessageHelper("AdMobNativeAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, CGSize.zero, false)
        super.init()
        _helper = BannerAdHelper(_bridge, self, _messageHelper)
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

    func createAdView() -> GADUnifiedNativeAdView? {
        guard let nibObjects = Bundle(for: GADUnifiedNativeAdView.self).loadNibNamed(_layoutName,
                                                                                     owner: nil,
                                                                                     options: nil) else {
            assert(false, "Invalid layout")
            return nil
        }
        guard let view = nibObjects[0] as? GADUnifiedNativeAdView else {
            assert(false, "Invalid layout class")
            return nil
        }
        return view
    }

    func createView() {
        Thread.runOnMainThread {
            assert(self._view == nil)
            guard let view = self.createAdView() else {
                assert(false, "Cannot create ad view")
                return
            }
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
                return
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
        Thread.runOnMainThread {
            guard let view = self._view else {
                assert(false, "Ad view is not initialized")
                return
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

            (view.starRatingView as? UIImageView)?.image = self.imageForStars(ad.starRating?.floatValue ?? 0)
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

            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }

    func adLoader(_ adLoader: GADAdLoader, didFailToReceiveAdWithError error: GADRequestError) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, EEJsonUtils.convertDictionary(toString: [
                "code": (error as NSError).code,
                "message": error.localizedDescription
            ]))
        }
    }

    func adLoaderDidFinishLoading(_ adLoader: GADAdLoader) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func nativeAdWillPresentScreen(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func nativeAdDidRecordImpression(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func nativeAdDidRecordClick(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func nativeAdWillLeaveApplication(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }

    func nativeAdWillDismissScreen(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }

    func nativeAdDidDismissScreen(_ nativeAd: GADUnifiedNativeAd) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function)")
        }
    }
}

 */
