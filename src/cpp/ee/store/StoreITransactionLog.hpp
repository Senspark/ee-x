//
//  StoreITransactionLog.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_TRANSACTION_LOG_HPP
#define EE_X_STORE_I_TRANSACTION_LOG_HPP

#ifdef __cplusplus

#include <string>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class ITransactionLog {
public:
    virtual ~ITransactionLog() = default;

    virtual void clear() = 0;
    virtual bool hasRecordOf(const std::string& transactionId) const = 0;
    virtual void record(const std::string& transactionId) = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_TRANSACTION_LOG_HPP */
