//
//  FacebookAds.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import FBAudienceNetwork
import RxSwift

private let kTag = "\(FacebookAdsBridge.self)"
private let kPrefix = "FacebookAdsBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kGetTestDeviceHash = "\(kPrefix)GetTestDeviceHash"
private let kAddTestDevice = "\(kPrefix)AddTestDevice"
private let kClearTestDevices = "\(kPrefix)ClearTestDevices"
private let kGetBannerAdSize = "\(kPrefix)GetBannerAdSize"
private let kCreateBannerAd = "\(kPrefix)CreateBannerAd"
private let kCreateNativeAd = "\(kPrefix)CreateNativeAd"
private let kCreateInterstitialAd = "\(kPrefix)CreateInterstitialAd"
private let kCreateRewardedAd = "\(kPrefix)CreateRewardedAd"
private let kDestroyAd = "\(kPrefix)DestroyAd"

@objc(EEFacebookAdsBridge)
class FacebookAdsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false
    private let _bannerHelper = FacebookBannerHelper()
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
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kGetTestDeviceHash) { _ in
            self.testDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message in
            self.addTestDevice(message)
            return ""
        }
        _bridge.registerHandler(kClearTestDevices) { _ in
            self.clearTestDevice()
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
        _bridge.registerHandler(kCreateInterstitialAd) { message in
            Utils.toString(self.createInterstitialAd(message))
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
        _bridge.deregisterHandler(kGetTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
        _bridge.deregisterHandler(kClearTestDevices)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kCreateNativeAd)
        _bridge.deregisterHandler(kCreateInterstitialAd)
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
                self._initializing = true
                FBAudienceNetworkAds.initialize(with: nil) { result in
                    Thread.runOnMainThread {
                        self._logger.debug("\(kTag): initialize: result = \(result.isSuccess) message = \(result.message)")
                        self._initializing = false
                        self._initialized = true
                        if result.isSuccess {
                            FBAdSettings.setAdvertiserTrackingEnabled(true)
                        } else {
                            self._logger.error("\(kTag): initialize: result = \(result.isSuccess)")
                        }
                        single(.success(true))
                    }
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }

    var testDeviceHash: String {
        return FBAdSettings.testDeviceHash()
    }

    func addTestDevice(_ hash: String) {
        Thread.runOnMainThread {
            FBAdSettings.addTestDevice(hash)
        }
    }

    func clearTestDevice() {
        Thread.runOnMainThread {
            self.checkInitialized()
            FBAdSettings.clearTestDevices()
        }
    }

    func createBannerAd(_ adId: String, _ size: FBAdSize) -> Bool {
        return createAd(adId) {
            FacebookBannerAd(_bridge, _logger, adId, size, _bannerHelper)
        }
    }

    func createNativeAd(_ adId: String, _ layoutName: String) -> Bool {
        return createAd(adId) {
            FacebookNativeAd(_bridge, _logger, adId, layoutName)
        }
    }

    func createInterstitialAd(_ adId: String) -> Bool {
        return createAd(adId) {
            FacebookInterstitialAd(self._bridge, self._logger, adId)
        }
    }

    func createRewardedAd(_ adId: String) -> Bool {
        return createAd(adId) {
            FacebookRewardedAd(self._bridge, self._logger, adId)
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
}
