//
//  EEShaderManager.hpp
//  ee-library
//
//  Created by Zinge on 8/15/17.
//
//

#ifndef EE_LIBRARY_SHADER_MANAGER_HPP
#define EE_LIBRARY_SHADER_MANAGER_HPP

#include <map>
#include <string>

#include "EECocos2dxFwd.hpp"

namespace ee {
class Shader;

class ShaderManager final {
private:
    using Self = ShaderManager;

public:
    static Self& getInstance();

    ShaderManager() = default;
    ~ShaderManager() = default;

    ShaderManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    /// Adds a shader.
    /// @param[in] name The program name.
    /// @param[in] vertex The vertex shader content.
    /// @param[in] fragment The fragment shader content.
    void addShader(const std::string& name, const std::string& vertex,
                   const std::string& fragment);

    /// Checks whether the specified program is added.
    /// @param[in] name The program name.
    bool isAdded(const std::string& name) const;

    /// Gets the GL program.
    /// @param[in] name The program name.
    /// @return nullptr if the program doesn't exist.
    cocos2d::GLProgram* getProgram(const std::string& name) const;

    /// Creates a GL program state.
    /// @param[in] name The program name.
    /// @return nullptr if the program doesn't exist.
    cocos2d::GLProgramState* createProgramState(const std::string& name) const;

private:
    std::map<std::string, std::unique_ptr<Shader>> shaders_;
};
} // namespace ee

#endif /* EE_LIBRARY_SHADER_MANAGER_HPP */
