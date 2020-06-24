//
//  AdMob.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

private let kPrefix = "AdMob"
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

@objc(EEAdMob)
class AdMob: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _bannerHelper = AdMobBannerHelper()
    private var _testDevices: [String] = []
    private var _bannerAds: [String: AdMobBannerAd] = [:]
    private var _nativeAds: [String: AdMobNativeAd] = [:]
    private var _interstitialAds: [String: AdMobInterstitialAd] = [:]
    private var _rewardedAds: [String: AdMobRewardedAd] = [:]

    required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
        registerHandlers()
    }

    func destroy() {
        deregisterHandlers()
        _bannerAds.values.forEach { $0.destroy() }
        _bannerAds.removeAll()
        _nativeAds.values.forEach { $0.destroy() }
        _nativeAds.removeAll()
        _interstitialAds.values.forEach { $0.destroy() }
        _interstitialAds.removeAll()
        _rewardedAds.values.forEach { $0.destroy() }
        _rewardedAds.removeAll()
    }

    func registerHandlers() {
        _bridge.registerHandler(kInitialize) { _ in
            self.initialize()
            return ""
        }
        _bridge.registerHandler(kGetEmulatorTestDeviceHash) { _ in
            self.emulatorTestDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message in
            self.addTestDevice(message)
            return ""
        }
        _bridge.registerHandler(kGetBannerAdSize) { message in
            let sizeId = Int(message) ?? -1
            let size = self.getBannerAdSize(sizeId)
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        _bridge.registerHandler(kCreateBannerAd) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let adId = dict["ad_id"] as? String else {
                assert(false, "Invalid argument")
            }
            guard let sizeId = dict["ad_size"] as? Int else {
                assert(false, "Invalid argument")
            }
            let adSize = self._bannerHelper.getAdSize(sizeId)
            return Utils.toString(self.createBannerAd(adId, adSize))
        }
        _bridge.registerHandler(kDestroyBannerAd) { message in
            Utils.toString(self.destroyBannerAd(message))
        }
        _bridge.registerHandler(kCreateNativeAd) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let adId = dict["ad_id"] as? String else {
                assert(false, "Invalid argument")
            }
            guard let layoutName = dict["layout_name"] as? String else {
                assert(false, "Invalid argument")
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
    }

    func initialize() {
        Thread.runOnMainThread {
            GADMobileAds.sharedInstance().start(completionHandler: nil)
        }
    }

    var emulatorTestDeviceHash: String {
        return kGADSimulatorID as? String ?? ""
    }

    func addTestDevice(_ hash: String) {
        Thread.runOnMainThread {
            self._testDevices.append(hash)
            GADMobileAds.sharedInstance().requestConfiguration.testDeviceIdentifiers = self._testDevices
        }
    }

    func getBannerAdSize(_ sizeId: Int) -> CGSize {
        return _bannerHelper.getSize(index: sizeId)
    }

    func createBannerAd(_ adId: String, _ size: GADAdSize) -> Bool {
        if _bannerAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobBannerAd(_bridge, adId, size)
        _bannerAds[adId] = ad
        return true
    }

    func destroyBannerAd(_ adId: String) -> Bool {
        guard let ad = _bannerAds[adId] else {
            return false
        }
        ad.destroy()
        _bannerAds.removeValue(forKey: adId)
        return true
    }

    func createNativeAd(_ adId: String, _ layoutName: String) -> Bool {
        if _nativeAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobNativeAd(_bridge, adId, layoutName)
        _nativeAds[adId] = ad
        return true
    }

    func destroyNativeAd(_ adId: String) -> Bool {
        guard let ad = _nativeAds[adId] else {
            return false
        }
        ad.destroy()
        _nativeAds.removeValue(forKey: adId)
        return true
    }

    func createInterstitialAd(_ adId: String) -> Bool {
        if _interstitialAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobInterstitialAd(_bridge, adId)
        _interstitialAds[adId] = ad
        return true
    }

    func destroyInterstitialAd(_ adId: String) -> Bool {
        guard let ad = _interstitialAds[adId] else {
            return false
        }
        ad.destroy()
        _interstitialAds.removeValue(forKey: adId)
        return true
    }

    func createRewardedAd(_ adId: String) -> Bool {
        if _rewardedAds.contains(where: { key, _ in key == adId }) {
            return false
        }
        let ad = AdMobRewardedAd(_bridge, adId)
        _rewardedAds[adId] = ad
        return true
    }

    func destroyRewardedAd(_ adId: String) -> Bool {
        guard let ad = _rewardedAds[adId] else {
            return false
        }
        ad.destroy()
        _rewardedAds.removeValue(forKey: adId)
        return true
    }
}
