//
//  IronSourceBannerAd.swift
//  ee-x-d542b565
//
//  Created by eps on 1/25/21.
//

private let kTag = "\(IronSourceBannerAd.self)"

internal class IronSourceBannerAd: NSObject, IBannerAd, ISBannerDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _adId: String
    private let _adSize: ISBannerSize
    private let _messageHelper: MessageHelper
    private var _helper: BannerAdHelper?
    private let _viewHelper: ViewHelper
    private var _isLoaded = false
    private var _ad: ISBannerView?

    init(_ bridge: IMessageBridge,
         _ logger: ILogger,
         _ adId: String,
         _ adSize: ISBannerSize,
         _ bannerHelper: IronSourceBannerHelper) {
        _bridge = bridge
        _logger = logger
        _adId = adId
        _adSize = adSize
        _messageHelper = MessageHelper("IronSourceBannerAd", _adId)
        _viewHelper = ViewHelper(CGPoint.zero, bannerHelper.getSize(adSize: adSize), false)
        super.init()
        _helper = BannerAdHelper(_bridge, self, _messageHelper)
        IronSource.setBannerDelegate(self)
        registerHandlers()
        createInternalAd()
    }

    func destroy() {
        deregisterHandler()
        destroyInternalAd()
        // Cannot clear delegates.
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
        }
    }

    func destroyInternalAd() {
        Thread.runOnMainThread {
            guard let ad = self._ad else {
                return
            }
            self._isLoaded = false
            IronSource.destroyBanner(ad)
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
            guard let rootView = Utils.getCurrentRootViewController() else {
                assert(false, "Root view is null")
                return
            }
            IronSource.loadBanner(with: rootView, size: self._adSize, placement: self._adId)
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

    func bannerDidLoad(_ bannerView: ISBannerView!) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._ad = bannerView
            self._viewHelper.view = bannerView
            self._isLoaded = true
            self._bridge.callCpp(self._messageHelper.onLoaded)
        }
    }

    func bannerDidFailToLoadWithError(_ error: Error!) {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId) message = \(error.localizedDescription)")
            self._bridge.callCpp(self._messageHelper.onFailedToLoad, error.localizedDescription)
        }
    }

    func didClickBanner() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
            self._bridge.callCpp(self._messageHelper.onClicked)
        }
    }

    func bannerWillPresentScreen() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }

    func bannerDidDismissScreen() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }

    func bannerWillLeaveApplication() {
        Thread.runOnMainThread {
            self._logger.debug("\(kTag): \(#function): id = \(self._adId)")
        }
    }
}
