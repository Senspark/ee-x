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

class JSONUtil {
    /// Example: let outputJson = JSONUtil.toJSON(adPaidData) ?? ""
    /// https://developer.apple.com/documentation/foundation/jsonencoder
    static func toJSON<T: Encodable>(_ object: T) -> String? {
        let encoder = JSONEncoder()
        // encoder.outputFormatting = .prettyPrinted
        guard let data = try? encoder.encode(object) else { return nil }
        return String(data: data, encoding: .utf8)
    }

    /// Example: JSONUtil.fromJSON(message, toType: AdRevenueData.self)
    /// https://developer.apple.com/documentation/foundation/jsondecoder
    static func fromJSON<T: Decodable>(_ jsonString: String, toType type: T.Type) -> T? {
        guard let data = jsonString.data(using: .utf8) else { return nil }
        let decoder = JSONDecoder()
        return try? decoder.decode(type, from: data)
    }
}

@_cdecl("ee_staticLog")
public func ee_staticLog(_ message: UnsafePointer<CChar>) {
    print(String(cString: message))
}
