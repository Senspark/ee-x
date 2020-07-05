//
//  AdMobBannerHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import GoogleMobileAds

internal class AdMobBannerHelper {
    private class func convertAdSizeToSize(_ adSize: GADAdSize) -> CGSize {
        let size = CGSizeFromGADAdSize(adSize)
        return CGSize(width: Double(Utils.convertDpToPixels(Float(size.width))),
                      height: Double(Utils.convertDpToPixels(Float(size.height))))
    }

    private var _indexToSize: [Int: CGSize] = [:]

    init() {
        for index in 0...3 {
            let adSize = getAdSize(index)
            let size = AdMobBannerHelper.convertAdSizeToSize(adSize)
            _indexToSize[index] = size
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
        assert(false, "Invalid ad index")
        return kGADAdSizeInvalid
    }

    private func getIndex(_ adSize: GADAdSize) -> Int {
        if GADAdSizeEqualToSize(adSize, kGADAdSizeBanner) {
            return 0
        }
        if GADAdSizeEqualToSize(adSize, kGADAdSizeLargeBanner) {
            return 1
        }
        if GADAdSizeEqualToSize(adSize, kGADAdSizeSmartBannerLandscape) ||
            GADAdSizeEqualToSize(adSize, kGADAdSizeSmartBannerPortrait) {
            return 2
        }
        if GADAdSizeEqualToSize(adSize, kGADAdSizeMediumRectangle) {
            return 3
        }
        assert(false, "Invalid ad size")
        return 0
    }

    func getSize(index: Int) -> CGSize {
        guard let size = _indexToSize[index] else {
            assert(false, "Invalid ad index")
            return CGSize.zero
        }
        return size
    }

    func getSize(adSize: GADAdSize) -> CGSize {
        return getSize(index: getIndex(adSize))
    }
}
