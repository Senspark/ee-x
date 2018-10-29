//
//  EEDataInfo.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_INFO_HPP_
#define EE_LIBRARY_DATA_INFO_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "EEUtils.hpp"
#include "EEDataMeta.hpp"
#include "EEDataUtils.hpp"

namespace ee {
/// @param Keys The format types.
template <class... Keys> struct DataFormat {
public:
    using KeyTypes = std::tuple<Keys...>;

    static std::string createKey(const Keys&... keys) {
        return toString(
            toString("___", detail::convert_enum_to_integer(keys))...);
    }
};

template <class... Keys>
struct DataFormat<std::tuple<Keys...>> : DataFormat<Keys...> {};

/// Use nil format.
/// Use default traits.
template <std::size_t DataId, class Value>
struct DataInfo<DataId, Value>
    : DataInfo<DataId, Value, DataFormat<>, DataTraits<Value>, void> {};

/// Use custom format.
/// Use default traits.
template <std::size_t DataId, class Value, class... Keys>
struct DataInfo<DataId, Value, DataFormat<Keys...>>
    : DataInfo<DataId, Value, DataFormat<Keys...>, DataTraits<Value>, void> {};

/// Use custom traits.
template <std::size_t DataId, class Value, class Traits>
struct DataInfo<DataId, Value, Traits,
                std::enable_if_t<detail::is_traits_v<Traits, Value>>>
    : DataInfo<DataId, Value, DataFormat<>, Traits, void> {};

/// Backward compability.
template <std::size_t DataId, class Value, class... Args>
struct DataInfo : DataInfo<DataId, Value, detail::get_format_t<Value, Args...>,
                           detail::get_traits_t<Value, Args...>, void> {};

/// Use custom format.
/// Use custom traits.
template <std::size_t DataId, class Value, class... Keys, class Traits>
struct DataInfo<DataId, Value, DataFormat<Keys...>, Traits,
                std::enable_if_t<detail::is_traits_v<Traits, Value>>>
    : DataFormat<Keys...>, Traits {
private:
    using Self = DataInfo;

public:
    static_assert(detail::is_traits_v<Traits, Value>,
                  "Traits type is not a valid data traits!");

    static_assert(not std::is_reference<Value>::value,
                  "Value type must not be a reference type!");

    static_assert(std::is_same<std::decay_t<Value>, Value>::value,
                  "Value type must be a decayed type!");

    using ValueType = Value;
    using FormatType = DataFormat<Keys...>;
    using TraitsType = Traits;

    using SetterType = std::function<void(ValueType& value)>;

    enum { Id = DataId };

    using FormatType::createKey;
    using TraitsType::store;
    using TraitsType::load;

    template <class ValueT, class... KeysT>
    static decltype(auto) set(ValueT&& value, KeysT&&... keys) {
        return ::ee::set<Self>(std::forward<ValueT>(value),
                               std::forward<KeysT>(keys)...);
    }

    template <class... KeysT> static decltype(auto) get(KeysT&&... keys) {
        return ::ee::get<Self>(std::forward<KeysT>(keys)...);
    }

    template <class... KeysT>
    static decltype(auto) getAndSet(const SetterType& setter, KeysT&&... keys) {
        return ::ee::getAndSet<Self>(setter, std::forward<KeysT>(keys)...);
    }
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_INFO_HPP_ */
