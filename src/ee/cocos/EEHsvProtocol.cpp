//
//  EEHSVProtocol.cpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#include "EEHsvProtocol.hpp"
#include "EEShaderUtils.hpp"

namespace ee {
HsvProtocol::HsvProtocol() {
    hue_ = 0.0f;
    saturation_ = 1.0f;
    brightness_ = 0.0f;
    contrast_ = 1.0f;

    matrixDirty_ = true;
    hueMatrixDirty_ = true;
    saturationMatrixDirty_ = true;
    brightnessMatrixDirty_ = true;
    contrastMatrixDirty_ = true;
}

void HsvProtocol::setHue(float hue) {
    if (hue == hue_) {
        return;
    }
    hue_ = hue;
    matrixDirty_ = hueMatrixDirty_ = true;
}

void HsvProtocol::setSaturation(float saturation) {
    if (saturation == saturation_) {
        return;
    }
    saturation_ = saturation;
    matrixDirty_ = saturationMatrixDirty_ = true;
}

void HsvProtocol::setBrightness(float brightness) {
    if (brightness == brightness_) {
        return;
    }
    brightness_ = brightness;
    matrixDirty_ = brightnessMatrixDirty_ = true;
}

void HsvProtocol::setContrast(float contrast) {
    if (contrast == contrast_) {
        return;
    }
    contrast_ = contrast;
    matrixDirty_ = contrastMatrixDirty_ = true;
}

const cocos2d::Mat4& HsvProtocol::getHsvMatrix() const {
    return hsvMatrix_;
}

bool HsvProtocol::updateMatrix() {
    updateHueMatrix();
    updateSaturationMatrix();
    updateBrightnessMatrix();
    updateContrastMatrix();

    if (matrixDirty_) {
        matrixDirty_ = false;
        hsvMatrix_.setIdentity();
        cocos2d::Mat4::multiply(hueMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(saturationMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(brightnessMatrix_, hsvMatrix_, &hsvMatrix_);
        cocos2d::Mat4::multiply(contrastMatrix_, hsvMatrix_, &hsvMatrix_);
        return true;
    }
    return false;
}

void HsvProtocol::updateHueMatrix() {
    if (hueMatrixDirty_) {
        hueMatrixDirty_ = false;
        hueMatrix_ = createHueMatrix(hue_);
    }
}

void HsvProtocol::updateSaturationMatrix() {
    if (saturationMatrixDirty_) {
        saturationMatrixDirty_ = false;
        saturationMatrix_ = createSaturationMatrix(saturation_);
    }
}

void HsvProtocol::updateBrightnessMatrix() {
    if (brightnessMatrixDirty_) {
        brightnessMatrixDirty_ = false;
        brightnessMatrix_ = createBrightnessMatrix(brightness_);
    }
}

void HsvProtocol::updateContrastMatrix() {
    if (contrastMatrixDirty_) {
        contrastMatrixDirty_ = false;
        contrastMatrix_ = createContrastMatrix(contrast_);
    }
}
} // namespace ee
