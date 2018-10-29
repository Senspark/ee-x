//
//  EEDataHandler.hpp
//  ee-library
//
//  Created by Zinge on 5/14/16.
//
//

#ifndef EE_LIBRARY_DATA_HANDLER_HPP_
#define EE_LIBRARY_DATA_HANDLER_HPP_

#include "EEForward.hpp"
#include "EEDataUtils.hpp"
#include "EEDataInfo.hpp"
#include "EEDataTraits.hpp"

namespace ee {
class DataHandler final {
private:
    using Self = DataHandler;

public:
    using SetCallback = std::function<void(
        std::size_t dataId, const std::string& key, const std::string& value)>;

    using GetCallback = std::function<bool(
        std::size_t dataId, const std::string& key, std::string& result)>;

    using RemoveCallback =
        std::function<void(std::size_t dataId, const std::string& key)>;

    static const int LowestPriority;

    explicit DataHandler(int priority = LowestPriority);

    ~DataHandler();

    DataHandler(const DataHandler& other) = delete;
    DataHandler& operator=(const DataHandler& other) = delete;

    DataHandler(DataHandler&& other) = delete;
    DataHandler& operator=(DataHandler&& other) = delete;

    /// Sets the dispatch priority for this handler.
    /// Lower-value priority handlers will dispatch first.
    /// @param[in] priority The desired priority.
    /// @return Instance to this.
    Self& setPriority(int priority);

    template <class DataType, class Value, class... Keys>
    [[deprecated]] decltype(auto) set(Value&& value, Keys&&... keys) {
        return ::ee::set<DataType>(std::forward<Value>(value),
                                   std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] decltype(auto) get(Keys&&... keys) {
        return ::ee::get<DataType>(std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] decltype(auto)
    getAndSet(const typename DataType::SetterType& f, Keys&&... keys) {
        return ::ee::getAndSet<DataType>(f, std::forward<Keys>(keys)...);
    }

    template <class DataType, class... Keys>
    [[deprecated]] decltype(auto) remove(Keys&&... keys) {
        return ::ee::remove<DataType>(std::forward<Keys>(keys)...);
    }

    /// Sets the SET callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const SetCallback& callback);

    /// Sets the GET callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const GetCallback& callback);

    /// Sets the REMOVE callback.
    /// @param[in] callback The desired callback.
    /// @return Instance to this for chaining.
    Self& setCallback(const RemoveCallback& callback);

private:
    friend void detail::set0(std::size_t dataId, const std::string& key,
                             const std::string& value);
    friend bool detail::get0(std::size_t dataId, const std::string& key,
                             std::string& result);
    friend void detail::remove0(std::size_t dataId, const std::string& key);

    void insertHandler();
    void eraseHandler();

    int priority_;

    SetCallback setCallback_;
    GetCallback getCallback_;
    RemoveCallback removeCallback_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_HANDLER_HPP_ */
