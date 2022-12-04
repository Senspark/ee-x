//
//  AppLovinBridge.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

import AppLovinSDK
import RxSwift

private let kPrefix = "AppLovinBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetVerboseLogging = "\(kPrefix)SetVerboseLogging"
private let kSetMuted = "\(kPrefix)SetMuted"
private let kHasInterstitialAd = "\(kPrefix)HasInterstitialAd"
private let kLoadInterstitialAd = "\(kPrefix)LoadInterstitialAd"
private let kShowInterstitialAd = "\(kPrefix)ShowInterstitialAd"
private let kHasRewardedAd = "\(kPrefix)HasRewardedAd"
private let kLoadRewardedAd = "\(kPrefix)LoadRewardedAd"
private let kShowRewardedAd = "\(kPrefix)ShowRewardedAd"

@objc(EEAppLovinBridge)
class AppLovinBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false
    private var _sdk: ALSdk?
    private var _interstitialAd: ALInterstitialAd?
    private var _interstitialAdListener: AppLovinInterstitialAdListener?
    private var _rewardedAd: ALIncentivizedInterstitialAd?
    private var _rewardedAdListener: AppLovinRewardedAdListener?
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }
    
    public func destroy() {
        deregisterHandlers()
        Thread.runOnMainThread {
            if !self._initialized {
                return
            }
            self._sdk = nil
            self._interstitialAd?.adLoadDelegate = nil
            self._interstitialAd?.adDisplayDelegate = nil
            self._interstitialAd = nil
            self._rewardedAd?.adDisplayDelegate = nil
            self._rewardedAd?.adVideoPlaybackDelegate = nil
            self._rewardedAd = nil
        }
    }
    
    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message, resolver in
            self.initialize(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerHandler(kSetVerboseLogging) { message in
            self.setVerboseLogging(Utils.toBool(message))
            return ""
        }
        _bridge.registerHandler(kSetMuted) { message in
            self.setMuted(Utils.toBool(message))
            return ""
        }
        _bridge.registerHandler(kHasInterstitialAd) { _ in
            Utils.toString(self.hasInterstitialAd)
        }
        _bridge.registerHandler(kLoadInterstitialAd) { _ in
            self.loadInterstitialAd()
            return ""
        }
        _bridge.registerHandler(kShowInterstitialAd) { _ in
            self.showInterstitialAd()
            return ""
        }
        _bridge.registerHandler(kHasRewardedAd) { _ in
            Utils.toString(self.hasRewardedAd)
        }
        _bridge.registerHandler(kLoadRewardedAd) { _ in
            self.loadRewardedAd()
            return ""
        }
        _bridge.registerHandler(kShowRewardedAd) { _ in
            self.showRewardedAd()
            return ""
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetVerboseLogging)
        _bridge.deregisterHandler(kSetMuted)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }
    
    func checkInitialized() {
        if !_initialized {
            assert(false, "Please call initialize() first")
        }
    }
    
    func initialize(_ key: String) -> Single<Bool> {
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
                self._initializing = true
                guard let sdk = ALSdk.shared(withKey: key) else {
                    assert(false, "Invalid key")
                    single(.success(false))
                    return
                }
                sdk.initializeSdk(completionHandler: { _ in
                    Thread.runOnMainThread {
                        self._initializing = false
                        self._initialized = true
                        single(.success(true))
                    }
                })
                
                let interstitialAdListener = AppLovinInterstitialAdListener(self._bridge, self._logger)
                let interstitialAd = ALInterstitialAd(sdk: sdk)
                interstitialAd.adLoadDelegate = interstitialAdListener
                interstitialAd.adDisplayDelegate = interstitialAdListener
                
                let rewardedAdListener = AppLovinRewardedAdListener(self._bridge, self._logger)
                let rewardedAd = ALIncentivizedInterstitialAd(sdk: sdk)
                rewardedAd.adDisplayDelegate = rewardedAdListener
                rewardedAd.adVideoPlaybackDelegate = rewardedAdListener
                
                self._sdk = sdk
                self._interstitialAd = interstitialAd
                self._interstitialAdListener = interstitialAdListener
                self._rewardedAd = rewardedAd
                self._rewardedAdListener = rewardedAdListener
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func setVerboseLogging(_ enabled: Bool) {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard let sdk = self._sdk else { return }
            sdk.settings.isVerboseLoggingEnabled = enabled
        }
    }
    
    func setMuted(_ enabled: Bool) {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard let sdk = self._sdk else { return }
            sdk.settings.isMuted = enabled
        }
    }
    
    var hasInterstitialAd: Bool {
        return _interstitialAdListener?.isLoaded ?? false
    }
    
    func loadInterstitialAd() {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard
                let sdk = self._sdk,
                let listener = self._interstitialAdListener
            else { return }
            sdk.adService.loadNextAd(ALAdSize.interstitial, andNotify: listener)
        }
    }
    
    func showInterstitialAd() {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard let ad = self._interstitialAd else { return }
            ad.show()
        }
    }
    
    var hasRewardedAd: Bool {
        if !_initialized {
            return false
        }
        return _rewardedAdListener?.isLoaded ?? false
    }
    
    func loadRewardedAd() {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard let ad = self._rewardedAd else { return }
            ad.preloadAndNotify(self._rewardedAdListener)
        }
    }
    
    func showRewardedAd() {
        Thread.runOnMainThread {
            self.checkInitialized()
            guard let ad = self._rewardedAd else { return }
            ad.showAndNotify(self._rewardedAdListener)
        }
    }
}
