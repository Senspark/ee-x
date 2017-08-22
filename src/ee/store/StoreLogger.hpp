//
//  StoreLogger.hpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#ifndef EE_X_STORE_STORE_LOGGER_HPP_
#define EE_X_STORE_STORE_LOGGER_HPP_

#include <string>

namespace ee {
namespace store {
class StoreLogger {
public:
    static StoreLogger& getInstance();

    StoreLogger();
    virtual ~StoreLogger();

    StoreLogger(const StoreLogger&) = delete;
    StoreLogger& operator=(const StoreLogger&) = delete;

    virtual void logDebug(const std::string& tag,
                          const std::string& message) = 0;

    virtual void logError(const std::string& tag,
                          const std::string& message) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_STORE_LOGGER_HPP_ */
