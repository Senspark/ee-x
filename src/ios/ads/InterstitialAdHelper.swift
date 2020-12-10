//
//  InterstitialAdHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

class InterstitialAdHelper {
    private let _bridge: IMessageBridge
    private let _ad: IInterstitialAd
    private let _helper: MessageHelper

    init(_ bridge: IMessageBridge,
         _ ad: IInterstitialAd,
         _ helper: MessageHelper) {
        _bridge = bridge
        _ad = ad
        _helper = helper
    }

    func registerHandlers() {
        _bridge.registerHandler(_helper.isLoaded) { _ in
            Utils.toString(self._ad.isLoaded)
        }
        _bridge.registerHandler(_helper.load) { _ in
            self._ad.load()
            return ""
        }
        _bridge.registerHandler(_helper.show) { _ in
            self._ad.show()
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded)
        _bridge.deregisterHandler(_helper.load)
        _bridge.deregisterHandler(_helper.show)
    }
}
