//
//  EEShader.cpp
//  ee-library
//
//  Created by Zinge on 8/15/17.
//
//

#include "EEShader.hpp"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerCustom.h>
#include <base/CCEventType.h>
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>
#include <renderer/CCGLProgramState.h>

namespace ee {
std::unique_ptr<Shader> Shader::create(const std::string& name,
                                       const std::string& vertex,
                                       const std::string& fragment) {
    return std::unique_ptr<Self>(new Self(name, vertex, fragment));
}

Shader::Shader(const std::string& name, const std::string& vertex,
               const std::string& fragment)
    : name_(name)
    , vertex_(vertex)
    , fragment_(fragment) {
    backgroundListener_ = cocos2d::EventListenerCustom::create(
        EVENT_RENDERER_RECREATED, std::bind(&Self::rebind, this));

    cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithFixedPriority(backgroundListener_, -1);
}

Shader::~Shader() {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(
        backgroundListener_);
}

cocos2d::GLProgram* Shader::getProgram() const {
    auto cache = cocos2d::GLProgramCache::getInstance();
    auto program = cache->getGLProgram(name_);
    if (program == nullptr) {
        program = cocos2d::GLProgram::createWithByteArrays(vertex_.c_str(),
                                                           fragment_.c_str());
        cache->addGLProgram(program, name_);
    }
    CC_ASSERT(program != nullptr);
    return program;
}

cocos2d::GLProgramState* Shader::createProgramState() const {
    return cocos2d::GLProgramState::create(getProgram());
}

void Shader::rebind() {
    auto cache = cocos2d::GLProgramCache::getInstance();
    auto program = cache->getGLProgram(name_);
    if (program != nullptr) {
        program->reset();
        program->initWithByteArrays(vertex_.c_str(), fragment_.c_str());
        program->link();
        program->updateUniforms();
    }
}
} // namespace ee
