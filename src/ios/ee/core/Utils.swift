//
//  Utils.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/3/20.
//

import Foundation

public class Utils: NSObject {
    #if os(iOS)
    @objc
    public class func getCurrentRootViewController() -> UIViewController? {
        return UIApplication.shared.keyWindow?.rootViewController
    }

    @objc
    public class func isLandscape() -> Bool {
        return UIApplication.shared.statusBarOrientation.isLandscape
    }
    #endif // os(iOS)

    @objc
    public class func toString(_ value: Bool) -> String {
        return value ? "true" : "false"
    }

    @objc
    public class func toBool(_ value: String) -> Bool {
        assert(value == "true" || value == "false", "Unexpected value: \(value)")
        return value == "true"
    }

    @objc
    public class func convertDpToPixels(_ dp: Float) -> Float {
        return dp * Platform.getDensity()
    }

    @objc
    public class func convertPixelsToDp(_ pixels: Float) -> Float {
        return pixels / Platform.getDensity()
    }
}

@_cdecl("ee_staticLog")
public func ee_staticLog(_ message: UnsafePointer<CChar>) {
    print(String(cString: message))
}
