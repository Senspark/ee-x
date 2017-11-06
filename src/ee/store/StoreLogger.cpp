//
//  StoreLogger.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include <stack>

#include "ee/store/StoreLogger.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/internal/SpinLock.hpp"

namespace ee {
namespace store {
namespace detail {
class DefaultStoreLogger : public StoreLogger {
public:
    virtual void logDebug(const std::string& tag,
                          const std::string& message) override {
        core::Logger::log(core::LogLevel::Debug, tag, message);
    }

    virtual void logError(const std::string& tag,
                          const std::string& message) override {
        core::Logger::log(core::LogLevel::Error, tag, message);
    }
};
} // namespace detail

namespace {
core::SpinLock stackLock;
std::stack<StoreLogger*> stack;
detail::DefaultStoreLogger defaultLogger;

std::stack<StoreLogger*>& getLoggerStack() {
    return stack;
}
} // namespace

StoreLogger& StoreLogger::getInstance() {
    std::lock_guard<core::SpinLock> guard(stackLock);
    return *getLoggerStack().top();
}

StoreLogger::StoreLogger() {
    std::lock_guard<core::SpinLock> guard(stackLock);
    getLoggerStack().push(this);
}

StoreLogger::~StoreLogger() {
    std::lock_guard<core::SpinLock> guard(stackLock);
    getLoggerStack().pop();
}
} // namespace store
} // namespace ee
