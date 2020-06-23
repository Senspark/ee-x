package com.ee.ads

import androidx.annotation.AnyThread

class MessageHelper(private val _prefix: String,
                    private val _adId: String) {
    val isLoaded @AnyThread get() = "${_prefix}_isLoaded_${_adId}"
    val load @AnyThread get() = "${_prefix}_load_${_adId}"
    val onLoaded @AnyThread get() = "${_prefix}_onLoaded_${_adId}"
    val onFailedToLoad @AnyThread get() = "${_prefix}_onFailedToLoad_${_adId}"
    val show @AnyThread get() = "${_prefix}_show_${_adId}"
    val onFailedToShow @AnyThread get() = "${_prefix}_onFailedToShow_${_adId}"
    val onClicked @AnyThread get() = "${_prefix}_onClicked_${_adId}"
    val onClosed @AnyThread get() = "${_prefix}_onClosed_${_adId}"
    val getPosition @AnyThread get() = "${_prefix}_getPosition_${_adId}"
    val setPosition @AnyThread get() = "${_prefix}_setPosition_${_adId}"
    val getSize @AnyThread get() = "${_prefix}_getSize_${_adId}"
    val setSize @AnyThread get() = "${_prefix}_setSize_${_adId}"
    val isVisible @AnyThread get() = "${_prefix}_isVisible_${_adId}"
    val setVisible @AnyThread get() = "${_prefix}_setVisible_${_adId}"
    val createInternalAd @AnyThread get() = "${_prefix}_createInternalAd_${_adId}"
    val destroyInternalAd @AnyThread get() = "${_prefix}_destroyInternalAd_${_adId}"
}