//
//  StoreSessionTransactionLog.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_SESSION_TRANSACTION_LOG_HPP
#define EE_X_STORE_SESSION_TRANSACTION_LOG_HPP

#ifdef __cplusplus

#include <set>

#include "ee/store/StoreITransactionLog.hpp"

namespace ee {
namespace store {
class SessionTransactionLog : public ITransactionLog {
public:
    virtual void clear() override;
    virtual bool hasRecordOf(const std::string& transactionId) const override;
    virtual void record(const std::string& transactionId) override;

private:
    std::set<std::string> transactionIds_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_SESSION_TRANSACTION_LOG_HPP */
