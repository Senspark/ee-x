//
//  jsb_common.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#include "ee/jsb/core/jsb_core_common.hpp"

#include <vector>

#include <cocos/scripting/js-bindings/jswrapper/SeApi.h>

namespace ee {
namespace core {
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
