//
//  EEShader.hpp
//  ee-library
//
//  Created by Zinge on 8/15/17.
//
//

#ifndef EE_LIBRARY_SHADER_HPP
#define EE_LIBRARY_SHADER_HPP

#include "EECocos2dxFwd.hpp"

namespace ee {
/// Wraps a shader and automatically rebinds when the renderer is recreated on
/// Android.
class Shader final {
private:
    using Self = Shader;

public:
    /// Creates a shader.
    /// @param[in] name The program name.
    /// @param[in] vertex The vertex shader content.
    /// @param[in] fragment The fragment shader content.
    static std::unique_ptr<Self> create(const std::string& name,
                                        const std::string& vertex,
                                        const std::string& fragment);

    ~Shader();

    /// Gets the GL program.
    /// Automatically creates one if it has not created.
    cocos2d::GLProgram* getProgram() const;

    /// Creates a GL program state.
    cocos2d::GLProgramState* createProgramState() const;

protected:
    Shader(const std::string& name, const std::string& vertex,
           const std::string& fragment);

    Shader(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void rebind();

    std::string name_;
    std::string vertex_;
    std::string fragment_;

    cocos2d::EventListener* backgroundListener_;
};
} // namespace ee

#endif /* EE_LIBRARY_SHADER_HPP */
