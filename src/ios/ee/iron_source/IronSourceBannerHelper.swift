//
//  IronSourceBannerHelper.swift
//  ee-x-d542b565
//
//  Created by eps on 1/25/21.
//

internal class IronSourceBannerHelper {
    private class func convertAdSizeToSize(_ adSize: ISBannerSize) -> CGSize {
        return CGSize(width: Double(Utils.convertDpToPixels(Float(adSize.width))),
                      height: Double(Utils.convertDpToPixels(Float(adSize.height))))
    }

    private var _indexToSize: [Int: CGSize] = [:]

    init() {
        for index in 0...3 {
            let adSize = getAdSize(index)
            let size = IronSourceBannerHelper.convertAdSizeToSize(adSize)
            _indexToSize[index] = size
        }
    }

    func getAdSize(_ index: Int) -> ISBannerSize {
        if index == 0 {
            return ISBannerSize(description: kSizeBanner, width: 320, height: 50)
        }
        if index == 1 {
            return ISBannerSize(description: kSizeLarge, width: 320, height: 90)
        }
        if index == 2 {
            return ISBannerSize(description: kSizeRectangle, width: 300, height: 250)
        }
        assert(false, "Invalid ad index")
        return ISBannerSize(description: kSizeBanner, width: 320, height: 50)
    }

    private func getIndex(_ adSize: ISBannerSize) -> Int {
        if adSize.sizeDescription == kSizeBanner {
            return 0
        }
        if adSize.sizeDescription == kSizeLarge {
            return 1
        }
        if adSize.sizeDescription == kSizeRectangle {
            return 2
        }
        assert(false, "Invalid ad size")
        return 0
    }

    private func getSize(index: Int) -> CGSize {
        guard let size = _indexToSize[index] else {
            assert(false, "Invalid ad index")
            return CGSize.zero
        }
        return size
    }

    func getSize(adSize: ISBannerSize) -> CGSize {
        return getSize(index: getIndex(adSize))
    }
}
