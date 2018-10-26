//
//  EEImageBuilder.hpp
//  ee-library
//
//  Created by Zinge on 4/26/17.
//
//

#ifndef EE_LIBRARY_IMAGE_BUILDER_HPP_
#define EE_LIBRARY_IMAGE_BUILDER_HPP_

#include <string>

#include <platform/CCGL.h>

namespace ee {
class SceneSwitcher;
class ManagedScene;

class ImageBuilder {
private:
    using Self = ImageBuilder;

public:
    ImageBuilder();

    /// Sets the image name to be loaded.
    /// @param imageName The name of the image.
    /// @return This instance for chaining.
    Self& setImageName(const std::string& imageName);

    /// Sets the atlas name (.plist) to be loaded.
    /// @param imageName The name of the atlas.
    /// @return This instance for chaining.
    Self& setAtlasName(const std::string& atlasName);

    /// Sets the data filename (.json) and atlas filename (.atlas) for spine
    /// animation.
    Self& setSpineData(const std::string& dataName,
                       const std::string& atlasName);

    Self& setMipmapEnabled(bool enabled);

    /// Sets the minification filter, available values:
    /// - GL_NEAREST
    /// - GL_LINEAR
    /// - GL_NEAREST_MIPMAP_NEAREST
    /// - GL_LINEAR_MIPMAP_NEAREST
    /// - GL_NEAREST_MIPMAP_LINEAR
    /// - GL_LINEAR_MIPMAP_LINEAR
    /// Default is GL_LINEAR (anti-alias).
    /// @param minFilter Value of the min filter.
    /// @return This instance for chaining.
    Self& setMinFilter(GLuint minFilter);

    /// Sets the magnification filter, available values:
    /// - GL_NEAREST
    /// - GL_LINEAR
    /// Default is GL_LINEAR (anti-alias).
    /// @param magFilter Value of the mag filter.
    /// @return This instance for chaining.
    Self& setMagFilter(GLuint magFilter);

    /// Sets the wrapS value (horizontal axis), available values:
    /// - GL_CLAMP_TO_EDGE
    /// - GL_REPEAT
    /// Default is GL_CLAMP_TO_EDGE.
    /// @param wrapS Value of the wrapS.
    /// @return This instance for chaining.
    Self& setWrapS(GLuint wrapS);

    /// Sets the wrapT value (vertical axis), available values:
    /// - GL_CLAMP_TO_EDGE
    /// - GL_REPEAT
    /// Default is GL_CLAMP_TO_EDGE.
    /// @param wrapT Value of the wrapT.
    /// @return This instance for chaining.
    Self& setWrapT(GLuint wrapT);

private:
    friend SceneSwitcher;
    friend ManagedScene;

    bool useAtlas_;
    std::string imageName_;
    std::string atlasName_;
    bool useSpine_;
    std::string spineDataName_;
    std::string spineAtlasName_;
    bool mipmapEnabled_;
    GLuint minFilter_;
    GLuint magFilter_;
    GLuint wrapS_;
    GLuint wrapT_;
};
} // namespace ee

#endif /* EE_LIBRARY_IMAGE_BUILDER_HPP_ */
