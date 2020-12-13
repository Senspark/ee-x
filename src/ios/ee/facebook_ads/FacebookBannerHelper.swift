//
//  FacebookBannerHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import FBAudienceNetwork

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

    private var _indexToSize: [Int: CGSize] = [:]

    init() {
        for index in 0...4 {
            let adSize = getAdSize(index)
            let size = FacebookBannerHelper.convertAdSizeToSize(adSize)
            _indexToSize[index] = size
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

    private func getIndex(_ adSize: FBAdSize) -> Int {
        if adSize.size == kFBAdSizeHeight50Banner.size {
            return 0
        }
        if adSize.size == kFBAdSizeHeight90Banner.size {
            return 1
        }
        if adSize.size == kFBAdSizeInterstitial.size {
            return 2
        }
        if adSize.size == kFBAdSizeHeight250Rectangle.size {
            return 3
        }
        if adSize.size == kFBAdSize320x50.size {
            return 4
        }
        assert(false, "Invalid ad size")
        return 4
    }

    func getSize(index: Int) -> CGSize {
        guard let size = _indexToSize[index] else {
            assert(false, "Invalid size index")
            return CGSize.zero
        }
        return size
    }

    func getSize(adSize: FBAdSize) -> CGSize {
        return getSize(index: getIndex(adSize))
    }
}
