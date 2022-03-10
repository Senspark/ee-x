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
            return GADAdSizeBanner
        }
        if index == 1 {
            return GADAdSizeLargeBanner
        }
        if index == 2 {
            /*
             FIXME
             Not supported.
            if Utils.isLandscape() {
                return kGADAdSizeSmartBannerLandscape
            }
            return kGADAdSizeSmartBannerPortrait
             */
            return GADAdSizeInvalid
        }
        if index == 3 {
            return GADAdSizeMediumRectangle
        }
        assert(false, "Invalid ad index")
        return GADAdSizeInvalid
    }

    private func getIndex(_ adSize: GADAdSize) -> Int {
        if GADAdSizeEqualToSize(adSize, GADAdSizeBanner) {
            return 0
        }
        if GADAdSizeEqualToSize(adSize, GADAdSizeLargeBanner) {
            return 1
        }
        /*
         FIXME
         Not supported.
        if GADAdSizeEqualToSize(adSize, kGADAdSizeSmartBannerLandscape) ||
            GADAdSizeEqualToSize(adSize, kGADAdSizeSmartBannerPortrait) {
            return 2
        }
         */
        if GADAdSizeEqualToSize(adSize, GADAdSizeMediumRectangle) {
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
