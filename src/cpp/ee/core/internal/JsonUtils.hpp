//
//  JsonUtils.hpp
//  Pods
//
//  Created by eps on 2/2/21.
//

#ifndef EE_X_JSON_UTILS_HPP
#define EE_X_JSON_UTILS_HPP

#include <variant>

#include <ee/nlohmann/json_fwd.hpp>

#include "ee/core/CoreFwd.hpp"

namespace nlohmann {
template <typename... Args>
struct adl_serializer<std::variant<Args...>> {
    static void to_json(json& json, std::variant<Args...> const& value) {
        std::visit(
            [&json](auto&& v) { //
                json = std::forward<decltype(v)>(v);
            },
            value);
    }
};
} // namespace nlohmann

#endif /* EE_X_JSON_UTILS_HPP */
