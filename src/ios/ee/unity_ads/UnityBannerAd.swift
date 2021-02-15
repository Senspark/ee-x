//
//  UnityBannerAd.swift
//  Pods
//
//  Created by eps on 2/11/21.
//

import UnityAds

private let kTag = "\(UnityBannerAd.self)"

internal class UnityBannerAd: NSObject, IBannerAd, UADSBannerViewDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _adSize: CGSize
    private let _messageHelper: MessageHelper
    private var _helper: BannerAdHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: UADSBannerView?

    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String,
         _ adSize: CGSize,
         _ bannerHelper: UnityBannerHelper) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("UnityBannerAd", _adId)
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
            let ad = UADSBannerView(placementId: self._adId, size: self._adSize)
            ad.delegate = self
            self._ad = ad
            self._viewHelper.view = ad
            let rootView = Utils.getCurrentRootViewController()
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
            ad.load()
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

    func bannerViewDidLoad(_ bannerView: UADSBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }

    func bannerViewDidError(_ bannerView: UADSBannerView, error: UADSBannerError) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
        }
    }

    func bannerViewDidClick(_ bannerView: UADSBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }

    func bannerViewDidLeaveApplication(_ bannerView: UADSBannerView) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
}
