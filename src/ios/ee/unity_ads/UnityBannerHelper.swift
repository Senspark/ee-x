//
//  UnityBannerHelper.swift
//  Pods
//
//  Created by eps on 2/11/21.
//

import UnityAds

internal class UnityBannerHelper {
    private class func convertAdSizeToSize(_ adSize: CGSize) -> CGSize {
        return CGSize(width: Double(Utils.convertDpToPixels(Float(adSize.width))),
                      height: Double(Utils.convertDpToPixels(Float(adSize.height))))
    }

    private var _indexToSize: [Int: CGSize] = [:]

    init() {
        for index in 0...0 {
            let adSize = getAdSize(index)
            let size = UnityBannerHelper.convertAdSizeToSize(adSize)
            _indexToSize[index] = size
        }
    }

    func getAdSize(_ index: Int) -> CGSize {
        if index == 0 {
            return CGSize(width: 320, height: 50)
        }
        assert(false, "Invalid ad index")
        return CGSize(width: 320, height: 50)
    }

    private func getIndex(_ adSize: CGSize) -> Int {
        if adSize.width == 320, adSize.height == 50 {
            return 0
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

    func getSize(adSize: CGSize) -> CGSize {
        return getSize(index: getIndex(adSize))
    }
}
