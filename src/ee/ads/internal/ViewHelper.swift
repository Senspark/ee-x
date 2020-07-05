//
//  ViewHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

class ViewHelper {
    private class func applyPosition(_ view: UIView, _ value: CGPoint) {
        Thread.runOnMainThread {
            var frame = view.frame
            frame.origin = value
            view.frame = frame
        }
    }

    private class func applySize(_ view: UIView, _ value: CGSize) {
        Thread.runOnMainThread {
            var frame = view.frame
            frame.size = value
            view.frame = frame
        }
    }

    private class func applyVisible(_ view: UIView, _ value: Bool) {
        Thread.runOnMainThread {
            view.isHidden = !value
        }
    }

    private var _view: UIView?
    private var _position = CGPoint(x: 0, y: 0)
    private var _size = CGSize(width: 0, height: 0)
    private var _visible = false

    init(_ initialPosition: CGPoint,
         _ initialSize: CGSize,
         _ initialVisible: Bool) {
        position = initialPosition
        size = initialSize
        isVisible = initialVisible
    }

    var view: UIView? {
        get { return _view }
        set(value) {
            _view = value
            if let view = value {
                ViewHelper.applyPosition(view, position)
                ViewHelper.applySize(view, size)
                ViewHelper.applyVisible(view, isVisible)
            }
        }
    }

    var position: CGPoint {
        get { return _position }
        set(value) {
            let position = CGPoint(x: Double(Utils.convertPixelsToDp(Float(value.x))),
                                   y: Double(Utils.convertPixelsToDp(Float(value.y))))
            _position = position
            if let view = view {
                ViewHelper.applyPosition(view, position)
            }
        }
    }

    var size: CGSize {
        get { return _size }
        set(value) {
            let size = CGSize(width: Double(Utils.convertPixelsToDp(Float(value.width))),
                              height: Double(Utils.convertPixelsToDp(Float(value.height))))
            _size = size
            if let view = view {
                ViewHelper.applySize(view, size)
            }
        }
    }

    var isVisible: Bool {
        get { return _visible }
        set(value) {
            _visible = value
            if let view = view {
                ViewHelper.applyVisible(view, value)
            }
        }
    }
}
