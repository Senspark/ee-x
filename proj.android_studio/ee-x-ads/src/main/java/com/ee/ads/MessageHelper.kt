package com.ee.ads

class MessageHelper(private val _prefix: String,
                    private val _adId: String) {
    val isLoaded get() = "${_prefix}_isLoaded_${_adId}"
    val load get() = "${_prefix}_load_${_adId}"
    val onLoaded get() = "${_prefix}_onLoaded_${_adId}"
    val onFailedToLoad get() = "${_prefix}_onFailedToLoad_${_adId}"
    val show get() = "${_prefix}_show_${_adId}"
    val onFailedToShow get() = "${_prefix}_onFailedToShow_${_adId}"
    val onClicked get() = "${_prefix}_onClicked_${_adId}"
    val onClosed get() = "${_prefix}_onClosed_${_adId}"
    val getPosition get() = "${_prefix}_getPosition_${_adId}"
    val setPosition get() = "${_prefix}_setPosition_${_adId}"
    val getSize get() = "${_prefix}_getSize_${_adId}"
    val setSize get() = "${_prefix}_setSize_${_adId}"
    val isVisible get() = "${_prefix}_isVisible_${_adId}"
    val setVisible get() = "${_prefix}_setVisible_${_adId}"
    val createInternalAd get() = "${_prefix}_createInternalAd_${_adId}"
    val destroyInternalAd get() = "${_prefix}_destroyInternalAd_${_adId}"
}