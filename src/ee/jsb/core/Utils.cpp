//
//  Utils.cpp
//  ee-x
//
//  Created by eps on 2/3/20.
//

#include "ee/jsb/core/Utils.hpp"

#include <memory>
#include <vector>

#include <cocos/scripting/js-bindings/jswrapper/SeApi.h>

#if __has_include(<base/CCDirector.h>)
#define COCOS_CREATOR_VERSION 1
#else // _has_include(<base/CCDirector.h>)
#define COCOS_CREATOR_VERSION 2
#endif // _has_include(<base/CCDirector.h>)

#include <base/CCScheduler.h>
#if COCOS_CREATOR_VERSION == 1
#include <base/CCDirector.h>
#else // #if COCOS_CREATOR_VERSION == 1
#include <platform/CCApplication.h>
#endif // #if COCOS_CREATOR_VERSION == 1

namespace ee {
namespace core {
namespace {
constexpr cocos2d::Scheduler* getScheduler(cocos2d::Scheduler* scheduler) {
    return scheduler;
}

constexpr cocos2d::Scheduler* getScheduler(const std::shared_ptr<cocos2d::Scheduler>& scheduler) {
    // Version 2.1.3
    return scheduler.get();
}

/// Gets the cocos2d-x scheduler base on the current engine version.
cocos2d::Scheduler* getScheduler() {
#if COCOS_CREATOR_VERSION == 1
    return cocos2d::Director::getInstance()->getScheduler();
#else // COCOS_CREATOR_VERSION == 1
    auto&& scheduler = cocos2d::Application::getInstance()->getScheduler();
    return getScheduler(scheduler);
#endif // COCOS_CREATOR_VERSION == 1
}
} // namespace

void schedule(const std::function<void(float delta)>& callback, void* target,
              float interval, const std::string& key) {
    getScheduler()->schedule(callback, target, interval, false, key);
}

void runOnCocosThread(const std::function<void()>& callback) {
    getScheduler()->performFunctionInCocosThread(callback);
};

namespace {
/// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(std::string text, const std::string& delimiter) {
    std::vector<std::string> args;
    std::size_t pos = 0;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        auto token = text.substr(0, pos);
        args.push_back(token);
        text.erase(0, pos + delimiter.size());
    }
    args.push_back(text);
    return args;
}
} // namespace

bool getOrCreatePlainObject_r(const char* name, se::Object* parent,
                              se::Object** outObj) {
    assert(parent != nullptr);
    assert(outObj != nullptr);
    se::Value tmp;

    if (parent->getProperty(name, &tmp) && tmp.isObject()) {
        *outObj = tmp.toObject();
        (*outObj)->incRef();
    } else {
        *outObj = se::Object::createPlainObject();
        parent->setProperty(name, se::Value(*outObj));
    }

    return true;
}

se::Object* getPath(se::Object* parent, const std::string& path) {
    auto args = split(path, "/");
    se::Object* currentParent = parent;
    se::Object* current = nullptr;
    for (auto&& arg : args) {
        getOrCreatePlainObject_r(arg.c_str(), currentParent, &current);
        currentParent = current;
    }
    return current;
}
} // namespace core
} // namespace ee
