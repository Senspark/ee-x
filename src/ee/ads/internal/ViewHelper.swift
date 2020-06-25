//
//  ViewHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

class ViewHelper {
    private let _view: UIView
    private var _position = CGPoint(x: 0, y: 0)
    private var _size = CGSize(width: 0, height: 0)
    private var _visible = false

    init(_ view: UIView) {
        _view = view
    }

    var position: CGPoint {
        get { return _position }
        set(value) {
            Thread.runOnMainThread {
                let position = CGPoint(x: Double(Utils.convertPixelsToDp(Float(value.x))),
                                       y: Double(Utils.convertPixelsToDp(Float(value.y))))
                var frame = self._view.frame
                frame.origin = position
                self._view.frame = frame
                self._position = position
            }
        }
    }

    var size: CGSize {
        get { return _size }
        set(value) {
            Thread.runOnMainThread {
                let size = CGSize(width: Double(Utils.convertPixelsToDp(Float(value.width))),
                                  height: Double(Utils.convertPixelsToDp(Float(value.height))))
                var frame = self._view.frame
                frame.size = size
                self._view.frame = frame
                self._size = size
            }
        }
    }

    var isVisible: Bool {
        get { return _visible }
        set(value) {
            Thread.runOnMainThread {
                self._view.isHidden = !value
                self._visible = value
            }
        }
    }
}
