//
//  StoreAppleReceiptParser.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_APPLE_RECEIPT_PARSER_HPP
#define EE_X_STORE_APPLE_RECEIPT_PARSER_HPP

#include <string>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class AppleReceiptParser {
public:
    AppleReceiptParser();

    std::shared_ptr<AppleReceipt> parse(const std::string& receiptData);

private:
    IMessageBridge& bridge_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_APPLE_RECEIPT_PARSER_HPP */
