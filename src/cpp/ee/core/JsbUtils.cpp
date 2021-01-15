#include "ee/core/JsbUtils.hpp"

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

using Self = JsbUtils;

se::Object* Self::getPath(se::Object* parent, const std::string& path) {
    auto args = split(path, "/");
    se::Object* currentParent = parent;
    se::Object* current = nullptr;
    for (auto&& arg : args) {
        current = getChild(currentParent, arg);
        currentParent = current;
    }
    return current;
}

se::Object* Self::getChild(se::Object* parent, const std::string& name) {
    se::Object* child = nullptr;
    se::Value tmp;
    if (parent->getProperty(name.c_str(), &tmp) && tmp.isObject()) {
        child = tmp.toObject();
        child->incRef();
    } else {
        child = se::Object::createPlainObject();
        parent->setProperty(name.c_str(), se::Value(child));
    }
    return child;
} // namespace core
} // namespace core
} // namespace ee