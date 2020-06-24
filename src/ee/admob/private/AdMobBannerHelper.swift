//
//  AdMobBannerHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation
import GoogleMobileAds

class AdMobBannerHelper {
    class func convertAdSizeToSize(_ adSize: GADAdSize) -> CGSize {
        let size = CGSizeFromGADAdSize(adSize)
        return CGSize(width: Double(Utils.convertDpToPixels(Float(size.width))),
                      height: Double(Utils.convertDpToPixels(Float(size.height))))
    }

    private var _sizes: [Int: CGSize] = [:]

    init() {
        for i in 0...3 {
            let adSize = getAdSize(i)
            let size = AdMobBannerHelper.convertAdSizeToSize(adSize)
            _sizes[i] = size
        }
    }

    func getAdSize(_ index: Int) -> GADAdSize {
        if index == 0 {
            return kGADAdSizeBanner
        }
        if index == 1 {
            return kGADAdSizeLargeBanner
        }
        if index == 2 {
            if Utils.isLandscape() {
                return kGADAdSizeSmartBannerLandscape
            }
            return kGADAdSizeSmartBannerPortrait
        }
        if index == 3 {
            return kGADAdSizeMediumRectangle
        }
        assert(false, "Invalid ad size")
        return kGADAdSizeInvalid
    }

    func getSize(index: Int) -> CGSize {
        guard let size = _sizes[index] else {
            assert(false, "Invalid size index")
        }
        return size
    }
}
