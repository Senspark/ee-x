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
private let kGetBannerAdSize = "\(kPrefix)GetBannerAdSize"
private let kCreateBannerAd = "\(kPrefix)CreateBannerAd"
private let kDestroyBannerAd = "\(kPrefix)DestroyBannerAd"
private let kCreateNativeAd = "\(kPrefix)CreateNativeAd"
private let kDestroyNativeAd = "\(kPrefix)DestroyNativeAd"
private let kCreateInterstitialAd = "\(kPrefix)CreateInterstitialAd"
private let kDestroyInterstitialAd = "\(kPrefix)DestroyInterstitialAd"
private let kCreateRewardedAd = "\(kPrefix)CreateRewardedAd"
private let kDestroyRewardedAd = "\(kPrefix)DestroyRewardedAd"
private let kCreateAppOpenAd = "\(kPrefix)CreateAppOpenAd"
private let kDestroyAppOpenAd = "\(kPrefix)DestroyAppOpenAd"

@objc(EEAdMobBridge)
class AdMobBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false
    private let _bannerHelper = AdMobBannerHelper()
    private var _testDevices: [String] = []
    private var _bannerAds: [String: AdMobBannerAd] = [:]
    private var _nativeAds: [String: AdMobNativeAd] = [:]
    private var _interstitialAds: [String: AdMobInterstitialAd] = [:]
    private var _rewardedAds: [String: AdMobRewardedAd] = [:]
    private var _appOpenAds: [String: AdMobAppOpenAd] = [:]

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        _bannerAds.values.forEach { $0.destroy() }
        _bannerAds.removeAll()
        _nativeAds.values.forEach { $0.destroy() }
        _nativeAds.removeAll()
        _interstitialAds.values.forEach { $0.destroy() }
        _interstitialAds.removeAll()
        _rewardedAds.values.forEach { $0.destroy() }
        _rewardedAds.removeAll()
        _appOpenAds.values.forEach { $0.destroy() }
        _appOpenAds.removeAll()
    }

    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { _, resolver in
            self.initialize()
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
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
        _bridge.registerHandler(kDestroyBannerAd) { message in
            Utils.toString(self.destroyBannerAd(message))
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
        _bridge.registerHandler(kDestroyNativeAd) { message in
            Utils.toString(self.destroyNativeAd(message))
        }
        _bridge.registerHandler(kCreateInterstitialAd) { message in
            Utils.toString(self.createInterstitialAd(message))
        }
        _bridge.registerHandler(kDestroyInterstitialAd) { message in
            Utils.toString(self.destroyInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedAd) { message in
            Utils.toString(self.createRewardedAd(message))
        }
        _bridge.registerHandler(kDestroyRewardedAd) { message in
            Utils.toString(self.destroyRewardedAd(message))
        }
        _bridge.registerHandler(kCreateAppOpenAd) { message in
            Utils.toString(self.createAppOpenAd(message))
        }
        _bridge.registerHandler(kDestroyRewardedAd) { message in
            Utils.toString(self.destroyAppOpenAd(message))
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kGetEmulatorTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyBannerAd)
        _bridge.deregisterHandler(kCreateNativeAd)
        _bridge.deregisterHandler(kDestroyNativeAd)
        _bridge.deregisterHandler(kCreateInterstitialAd)
        _bridge.deregisterHandler(kDestroyInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedAd)
        _bridge.deregisterHandler(kDestroyRewardedAd)
        _bridge.deregisterHandler(kCreateAppOpenAd)
        _bridge.deregisterHandler(kDestroyAppOpenAd)
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
                    single(.success(false))
                    return
                }
                if self._initialized {
                    single(.success(true))
                    return
                }
                GADMobileAds.sharedInstance().start { status in
                    self._initializing = false
                    self._initialized = true
                    self._logger.info("\(kTag): initialize: done")
                    for (key, value) in status.adapterStatusesByClassName {
                        self._logger.info("\(kTag): adapter = \(key) state = \(value.state) latency = \(value.latency) description = \(value.description)")
                    }
                    single(.success(true))
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    var emulatorTestDeviceHash: String {
        return kGADSimulatorID as? String ?? ""
    }

    func addTestDevice(_ hash: String) {
        Thread.runOnMainThread {
            self.checkInitialized()
            self._testDevices.append(hash)
            GADMobileAds.sharedInstance().requestConfiguration.testDeviceIdentifiers = self._testDevices
        }
    }

    func createBannerAd(_ adId: String, _ adSize: GADAdSize) -> Bool {
        checkInitialized()
        if _bannerAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobBannerAd(_bridge, _logger, adId, adSize, _bannerHelper)
        _bannerAds[adId] = ad
        return true
    }

    func destroyBannerAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _bannerAds[adId] else {
            return false
        }
        ad.destroy()
        _bannerAds.removeValue(forKey: adId)
        return true
    }

    func createNativeAd(_ adId: String, _ layoutName: String) -> Bool {
        checkInitialized()
        if _nativeAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobNativeAd(_bridge, _logger, adId, layoutName)
        _nativeAds[adId] = ad
        return true
    }

    func destroyNativeAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _nativeAds[adId] else {
            return false
        }
        ad.destroy()
        _nativeAds.removeValue(forKey: adId)
        return true
    }

    func createInterstitialAd(_ adId: String) -> Bool {
        checkInitialized()
        if _interstitialAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobInterstitialAd(_bridge, _logger, adId)
        _interstitialAds[adId] = ad
        return true
    }

    func destroyInterstitialAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _interstitialAds[adId] else {
            return false
        }
        ad.destroy()
        _interstitialAds.removeValue(forKey: adId)
        return true
    }

    func createRewardedAd(_ adId: String) -> Bool {
        checkInitialized()
        if _rewardedAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobRewardedAd(_bridge, _logger, adId)
        _rewardedAds[adId] = ad
        return true
    }

    func destroyRewardedAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _rewardedAds[adId] else {
            return false
        }
        ad.destroy()
        _rewardedAds.removeValue(forKey: adId)
        return true
    }

    func createAppOpenAd(_ adId: String) -> Bool {
        checkInitialized()
        if _appOpenAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobAppOpenAd(_bridge, _logger, adId)
        _appOpenAds[adId] = ad
        return true
    }

    func destroyAppOpenAd(_ adId: String) -> Bool {
        checkInitialized()
        guard let ad = _appOpenAds[adId] else {
            return false
        }
        ad.destroy()
        _appOpenAds.removeValue(forKey: adId)
        return true
    }
}
