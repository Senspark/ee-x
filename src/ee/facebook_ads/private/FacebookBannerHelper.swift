//
//  FacebookBannerHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork
import Foundation

internal class FacebookBannerHelper {
    private class func getWidthInDp(_ adSize: FBAdSize) -> Float {
        let width = lrintf(Float(adSize.size.width))
        switch width {
        case 0, // Interstitial ad.
             -1: // Normal ad.
            return Float(UIScreen.main.bounds.size.width)
        default: // Deprecated ad.
            return Float(width)
        }
    }

    private class func getHeightInDp(_ adSize: FBAdSize) -> Float {
        let height = lrintf(Float(adSize.size.height))
        switch height {
        case 0: // Interstitial ad.
            return Float(UIScreen.main.bounds.size.height)
        default: // Normal ad.
            return Float(height)
        }
    }

    private class func convertAdSizeToSize(_ adSize: FBAdSize) -> CGSize {
        let size = CGSize(width: Double(getWidthInDp(adSize)),
                          height: Double(getHeightInDp(adSize)))
        return CGSize(width: Double(Utils.convertDpToPixels(Float(size.width))),
                      height: Double(Utils.convertDpToPixels(Float(size.height))))
    }

    private var _sizes: [Int: CGSize] = [:]

    init() {
        for i in 0...4 {
            let adSize = getAdSize(i)
            let size = FacebookBannerHelper.convertAdSizeToSize(adSize)
            _sizes[i] = size
        }
    }

    func getAdSize(_ index: Int) -> FBAdSize {
        if index == 0 {
            return kFBAdSizeHeight50Banner
        }
        if index == 1 {
            return kFBAdSizeHeight90Banner
        }
        if index == 2 {
            return kFBAdSizeInterstitial
        }
        if index == 3 {
            return kFBAdSizeHeight250Rectangle
        }
        if index == 4 {
            return kFBAdSize320x50
        }
        assert(false, "Invalid ad size")
        return kFBAdSize320x50
    }

    func getSize(_ index: Int) -> CGSize {
        guard let size = _sizes[index] else {
            assert(false, "Invalid size index")
        }
        return size
    }
}
