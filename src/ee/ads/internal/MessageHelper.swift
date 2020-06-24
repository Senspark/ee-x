//
//  MessageHelper.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

class MessageHelper {
    private let _prefix: String
    private let _adId: String

    init(_ prefix: String, _ adId: String) {
        _prefix = prefix
        _adId = adId
    }

    var isLoaded: String { return "\(_prefix)_isLoaded_\(_adId)" }
    var load: String { return "\(_prefix)_load_\(_adId)" }
    var onLoaded: String { return "\(_prefix)_onLoaded_\(_adId)" }
    var onFailedToLoad: String { return "\(_prefix)_onFailedToLoad_\(_adId)" }
    var show: String { return "\(_prefix)_show_\(_adId)" }
    var onFailedToShow: String { return "\(_prefix)_onFailedToShow_\(_adId)" }
    var onClicked: String { return "\(_prefix)_onClicked_\(_adId)" }
    var onClosed: String { return "\(_prefix)_onClosed_\(_adId)" }
    var getPosition: String { return "\(_prefix)_getPosition_\(_adId)" }
    var setPosition: String { return "\(_prefix)_setPosition_\(_adId)" }
    var getSize: String { return "\(_prefix)_getSize_\(_adId)" }
    var setSize: String { return "\(_prefix)_setSize_\(_adId)" }
    var isVisible: String { return "\(_prefix)_isVisible_\(_adId)" }
    var setVisible: String { return "\(_prefix)_setVisible_\(_adId)" }
    var createInternalAd: String { return "\(_prefix)_createInternalAd_\(_adId)" }
    var destroyInternalAd: String { return "\(_prefix)_destroyInternalAd_\(_adId)" }
}
