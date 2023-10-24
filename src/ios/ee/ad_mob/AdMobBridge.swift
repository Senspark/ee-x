//
//  AdMob.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import GoogleMobileAds
import RxSwift

private let kTag = "\(AdMobBridge.self)"
private let kPrefix = "AdMobBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kGetEmulatorTestDeviceHash = "\(kPrefix)GetEmulatorTestDeviceHash"
private let kAddTestDevice = "\(kPrefix)AddTestDevice"
private let kOpenTestSuite = "\(kPrefix)OpenTestSuite"
private let kGetBannerAdSize = "\(kPrefix)GetBannerAdSize"
private let kCreateBannerAd = "\(kPrefix)CreateBannerAd"
private let kCreateNativeAd = "\(kPrefix)CreateNativeAd"
private let kCreateAppOpenAd = "\(kPrefix)CreateAppOpenAd"
private let kCreateInterstitialAd = "\(kPrefix)CreateInterstitialAd"
private let kCreateRewardedInterstitialAd = "\(kPrefix)CreateRewardedInterstitialAd"
private let kCreateRewardedAd = "\(kPrefix)CreateRewardedAd"
private let kDestroyAd = "\(kPrefix)DestroyAd"
private let kOnAdPaid = "\(kPrefix)OnAdPaid"

@objc(EEAdMobBridge)
class AdMobBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false
    private let _bannerHelper = AdMobBannerHelper()
    private var _testDevices: [String] = []
    private var _ads: [String: IAd] = [:]

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        _ads.values.forEach { $0.destroy() }
        _ads.removeAll()
    }

    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { _, resolver in
            self.initialize()
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onFailure: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kGetEmulatorTestDeviceHash) { _ in
            self.emulatorTestDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message in
            self.addTestDevice(message)
            return ""
        }
        _bridge.registerHandler(kOpenTestSuite) { _ in
            self.openTestSuite()
            return ""
        }
        _bridge.registerHandler(kGetBannerAdSize) { message in
            let index = Int(message) ?? -1
            let size = self._bannerHelper.getSize(index: index)
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        _bridge.registerHandler(kCreateBannerAd) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let adId = dict["adId"] as? String,
                let index = dict["adSize"] as? Int
            else {
                assert(false, "Invalid argument")
                return ""
            }
            let adSize = self._bannerHelper.getAdSize(index)
            return Utils.toString(self.createBannerAd(adId, adSize))
        }
        _bridge.registerHandler(kCreateNativeAd) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let adId = dict["adId"] as? String,
                let layoutName = dict["layoutName"] as? String
            else {
                assert(false, "Invalid argument")
                return ""
            }
            return Utils.toString(self.createNativeAd(adId, layoutName))
        }
        _bridge.registerHandler(kCreateAppOpenAd) { message in
            Utils.toString(self.createAppOpenAd(message))
        }
        _bridge.registerHandler(kCreateInterstitialAd) { message in
            Utils.toString(self.createInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedInterstitialAd) { message in
            Utils.toString(self.createRewardedInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedAd) { message in
            Utils.toString(self.createRewardedAd(message))
        }
        _bridge.registerHandler(kDestroyAd) { message in
            Utils.toString(self.destroyAd(message))
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kGetEmulatorTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kCreateNativeAd)
        _bridge.deregisterHandler(kCreateAppOpenAd)
        _bridge.deregisterHandler(kCreateInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedAd)
        _bridge.deregisterHandler(kDestroyAd)
    }

    func checkInitialized() {
        if !_initialized {
            assert(false, "Please call initialize() first")
        }
    }

    func initialize() -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                if self._initializing {
                    self._logger.info("\(kTag): \(#function): initializing")
                    single(.success(false))
                    return
                }
                if self._initialized {
                    self._logger.info("\(kTag): \(#function): initialized")
                    single(.success(true))
                    return
                }
                GADMobileAds.sharedInstance().start { status in
                    Thread.runOnMainThread {
                        self._initializing = false
                        self._initialized = true
                        self._logger.info("\(kTag): initialize: done")
                        for (key, value) in status.adapterStatusesByClassName {
                            self._logger.info("\(kTag): adapter = \(key) state = \(value.state) latency = \(value.latency) description = \(value.description)")
                        }
                        single(.success(true))
                    }
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    var emulatorTestDeviceHash: String {
        return GADSimulatorID
    }

    func addTestDevice(_ hash: String) {
        Thread.runOnMainThread {
            self.checkInitialized()
            self._testDevices.append(hash)
            GADMobileAds.sharedInstance().requestConfiguration.testDeviceIdentifiers = self._testDevices
        }
    }

    func openTestSuite() {
        Thread.runOnMainThread {
            guard let rootView = Utils.getCurrentRootViewController() else {
                return
            }
            /*
             FIXME
            GoogleMobileAdsMediationTestSuite.present(on: rootView, delegate: nil)
             */
        }
    }

    func createBannerAd(_ adId: String, _ adSize: GADAdSize) -> Bool {
        return createAd(adId) {
            AdMobBannerAd(_bridge, _logger, adId, adSize, _bannerHelper, onAdPaid)
        }
    }

    func createNativeAd(_ adId: String, _ layoutName: String) -> Bool {
        return false
        /* FIXME:
         return createAd(adId) {
             AdMobNativeAd(_bridge, _logger, adId, layoutName)
         }
        */
    }

    func createAppOpenAd(_ adId: String) -> Bool {
        return createAd(adId) {
            AdMobAppOpenAd(self._bridge, self._logger, adId, onAdPaid)
        }
    }

    func createInterstitialAd(_ adId: String) -> Bool {
        return createAd(adId) {
            AdMobInterstitialAd(self._bridge, self._logger, adId, onAdPaid)
        }
    }

    func createRewardedInterstitialAd(_ adId: String) -> Bool {
        return createAd(adId) {
            AdMobRewardedInterstitialAd(self._bridge, self._logger, adId, onAdPaid)
        }
    }

    func createRewardedAd(_ adId: String) -> Bool {
        return createAd(adId) {
            AdMobRewardedAd(self._bridge, self._logger, adId, onAdPaid)
        }
    }

    func createAd(_ adId: String, _ creator: () -> IAd) -> Bool {
        checkInitialized()
        if _ads.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = creator()
        _ads[adId] = ad
        return true
    }

    func destroyAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _ads[adId] else {
            return false
        }
        ad.destroy()
        _ads.removeValue(forKey: adId)
        return true
    }
    
    func onAdPaid(adResponse:AdPaidResponse) -> Void {
        _logger.info("AppsFlyer log af: " + adResponse.valueMicros.stringValue)
        _bridge.callCpp(kOnAdPaid, Utils.toString(adResponse))

        // FIXME: Để đây để move vào logic chính

        let adRevenueParams:[AnyHashable: Any] = [
            kAppsFlyerAdRevenueAdUnit : adResponse.adUnitId,
            kAppsFlyerAdRevenueAdType : adResponse.adFormat,
        ]
        AppsFlyerAdRevenue.shared().logAdRevenue(
            monetizationNetwork: adResponse.networkName,
            mediationNetwork: MediationNetworkType.googleAdMob,
            eventRevenue: adResponse.valueMicros,
            revenueCurrency: "USD",additionalParameters: adRevenueParams)
    }
}
