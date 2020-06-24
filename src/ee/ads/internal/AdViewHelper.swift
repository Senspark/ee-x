//
//  AdViewHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

class AdViewHelper {
    private let _bridge: IMessageBridge
    private var _view: IAdView
    private let _helper: MessageHelper

    init(_ bridge: IMessageBridge,
         _ view: IAdView,
         _ helper: MessageHelper) {
        _bridge = bridge
        _view = view
        _helper = helper
    }

    func registerHandlers() {
        _bridge.registerHandler(_helper.isLoaded) { _ in
            Utils.toString(self._view.isLoaded)
        }
        _bridge.registerHandler(_helper.load) { _ in
            self._view.load()
            return ""
        }
        _bridge.registerHandler(_helper.getPosition) { _ in
            let position = self._view.position
            return EEJsonUtils.convertDictionary(toString: [
                "x": position.x,
                "y": position.y
            ])
        }
        _bridge.registerHandler(_helper.setPosition) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let x = dict["x"] as? Int else {
                assert(false, "Invalid argument")
                return ""
            }
            guard let y = dict["y"] as? Int else {
                assert(false, "Invalid argument")
                return ""
            }
            self._view.position = CGPoint(x: x, y: y)
            return ""
        }
        _bridge.registerHandler(_helper.getSize) { _ in
            let size = self._view.size
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        _bridge.registerHandler(_helper.setSize) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let width = dict["width"] as? Int else {
                assert(false, "Invalid argument")
                return ""
            }
            guard let height = dict["height"] as? Int else {
                assert(false, "Invalid argument")
                return ""
            }
            self._view.size = CGSize(width: width, height: height)
            return ""
        }
        _bridge.registerHandler(_helper.isVisible) { _ in
            Utils.toString(self._view.isVisible)
        }
        _bridge.registerHandler(_helper.setVisible) { message in
            self._view.isVisible = Utils.toBool(message)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded)
        _bridge.deregisterHandler(_helper.load)
        _bridge.deregisterHandler(_helper.getPosition)
        _bridge.deregisterHandler(_helper.setPosition)
        _bridge.deregisterHandler(_helper.getSize)
        _bridge.deregisterHandler(_helper.setSize)
        _bridge.deregisterHandler(_helper.isVisible)
        _bridge.deregisterHandler(_helper.setVisible)
    }
}
