//
//  StoreSessionTransactionLog.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreSessionTransactionLog.hpp"

namespace ee {
namespace store {
using Self = SessionTransactionLog;

void Self::clear() {
    transactionIds_.clear();
}

bool Self::hasRecordOf(const std::string& transactionId) const {
    return transactionIds_.count(transactionId) != 0;
}

void Self::record(const std::string& transactionId) {
    transactionIds_.insert(transactionId);
}
} // namespace store
} // namespace ee
