//
//  EEHSVProtocol.hpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#ifndef EE_LIBRARY_HSV_PROTOCOL_HPP_
#define EE_LIBRARY_HSV_PROTOCOL_HPP_

#include <math/Mat4.h>

namespace cocos2d {
class GLProgram;
} // namespace cocos2d

namespace ee {
class HsvProtocol {
public:
    HsvProtocol();
    virtual ~HsvProtocol() = default;

    /// Assigns the hue rotation (in degrees).
    ///
    /// Default is 0.0.
    void setHue(float hue);

    /// Default is 1.0.
    void setSaturation(float saturation);

    /// Default is 0.0.
    void setBrightness(float brightness);

    /// Default is 1.0.
    void setContrast(float contrast);

    float getHue() const { return hue_; }
    float getSaturation() const { return saturation_; }
    float getBrightness() const { return brightness_; }
    float getContrast() const { return contrast_; }

protected:
    virtual bool updateMatrix();

    const cocos2d::Mat4& getHsvMatrix() const;

private:
    void updateHueMatrix();
    void updateSaturationMatrix();
    void updateBrightnessMatrix();
    void updateContrastMatrix();

    float hue_;
    float saturation_;
    float brightness_;
    float contrast_;

    cocos2d::Mat4 hsvMatrix_;
    cocos2d::Mat4 hueMatrix_;
    cocos2d::Mat4 saturationMatrix_;
    cocos2d::Mat4 brightnessMatrix_;
    cocos2d::Mat4 contrastMatrix_;

    bool matrixDirty_;
    bool hueMatrixDirty_;
    bool saturationMatrixDirty_;
    bool brightnessMatrixDirty_;
    bool contrastMatrixDirty_;
};
} // namespace ee

#endif /* EE_LIBRARY_HSV_PROTOCOL_HPP_ */
