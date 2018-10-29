//
//  EEDataHandler.cpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#include <cassert>
#include <limits>
#include <map>
#include <unordered_set>

#include "EEDataHandler.hpp"
#include "EEDataUtils.hpp"

#include <base/ccMacros.h>

namespace ee {
namespace {
int dispatchingCounter_ = 0;
int constructingCounter_ = 0;

bool isDispatching() noexcept {
    return dispatchingCounter_ > 0;
}

bool isConstructing() noexcept {
    return constructingCounter_ > 0;
}

class DispatchGuard {
public:
    explicit DispatchGuard(int& counter)
        : counter_(counter) {
        lock();
    }

    void lock() { ++counter_; }

    void unlock() { --counter_; }

    ~DispatchGuard() { unlock(); }

private:
    int& counter_;
};

std::map<int, std::unordered_set<DataHandler*>> handlers_;
} // namespace

namespace detail {
void set0(std::size_t dataId, const std::string& key,
          const std::string& value) {
    CC_ASSERT(not isConstructing());
    DispatchGuard guard(dispatchingCounter_);
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->setCallback_) {
                handler->setCallback_(dataId, key, value);
            }
        }
    }
}

bool get0(std::size_t dataId, const std::string& key, std::string& result) {
    CC_ASSERT(not isConstructing());
    DispatchGuard guard(dispatchingCounter_);
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->getCallback_) {
                if (handler->getCallback_(dataId, key, result)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void remove0(std::size_t dataId, const std::string& key) {
    CC_ASSERT(not isConstructing());
    DispatchGuard guard(dispatchingCounter_);
    for (auto&& handlers : handlers_) {
        for (auto&& handler : handlers.second) {
            if (handler->removeCallback_) {
                handler->removeCallback_(dataId, key);
            }
        }
    }
}
} // namespace detail

const int DataHandler::LowestPriority = std::numeric_limits<int>::max();

DataHandler::DataHandler(int priority)
    : priority_(priority) {
    insertHandler();
}

DataHandler::~DataHandler() {
    eraseHandler();
}

void DataHandler::insertHandler() {
    CC_ASSERT(not isDispatching());
    DispatchGuard guard(constructingCounter_);
    CC_ASSERT(handlers_[priority_].count(this) == 0);
    handlers_[priority_].insert(this);
}

void DataHandler::eraseHandler() {
    CC_ASSERT(not isDispatching());
    DispatchGuard guard(constructingCounter_);
    assert(handlers_[priority_].count(this) != 0);
    handlers_[priority_].erase(this);
}

DataHandler& DataHandler::setPriority(int priority) {
    eraseHandler();
    priority_ = priority;
    insertHandler();
    return *this;
}

DataHandler& DataHandler::setCallback(const SetCallback& callback) {
    setCallback_ = callback;
    return *this;
}

DataHandler& DataHandler::setCallback(const GetCallback& callback) {
    getCallback_ = callback;
    return *this;
}

DataHandler& DataHandler::setCallback(const RemoveCallback& callback) {
    removeCallback_ = callback;
    return *this;
}
} // namespace ee
