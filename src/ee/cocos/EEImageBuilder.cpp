//
//  EEImageBuilder.cpp
//  ee-library
//
//  Created by Zinge on 4/26/17.
//
//

#include "EEImageBuilder.hpp"

namespace ee {
ImageBuilder::ImageBuilder() {
    useAtlas_ = false;
    useSpine_ = false;
    setMinFilter(GL_LINEAR);
    setMagFilter(GL_LINEAR);
    setWrapS(GL_CLAMP_TO_EDGE);
    setWrapT(GL_CLAMP_TO_EDGE);
}

ImageBuilder& ImageBuilder::setImageName(const std::string& imageName) {
    imageName_ = imageName;
    return *this;
}

ImageBuilder& ImageBuilder::setAtlasName(const std::string& atlasName) {
    useAtlas_ = true;
    atlasName_ = atlasName;
    return *this;
}

ImageBuilder& ImageBuilder::setSpineData(const std::string& dataName,
                                         const std::string& atlasName) {
    useSpine_ = true;
    spineDataName_ = dataName;
    spineAtlasName_ = atlasName;
    return *this;
}

ImageBuilder& ImageBuilder::setMipmapEnabled(bool enabled) {
    mipmapEnabled_ = enabled;
    return *this;
}

ImageBuilder& ImageBuilder::setMinFilter(GLuint minFilter) {
    if (minFilter == GL_NEAREST_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_LINEAR_MIPMAP_LINEAR) {
        setMipmapEnabled(true);
    } else {
        setMipmapEnabled(false);
    }
    minFilter_ = minFilter;
    return *this;
}

ImageBuilder& ImageBuilder::setMagFilter(GLuint magFilter) {
    magFilter_ = magFilter;
    return *this;
}

ImageBuilder& ImageBuilder::setWrapS(GLuint wrapS) {
    wrapS_ = wrapS;
    return *this;
}

ImageBuilder& ImageBuilder::setWrapT(GLuint wrapT) {
    wrapT_ = wrapT;
    return *this;
}
} // namespace ee
