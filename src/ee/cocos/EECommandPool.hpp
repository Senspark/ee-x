//
//  CommandPool.hpp
//  ee-library
//
//  Created by Zinge on 4/10/17.
//
//

#ifndef EE_LIBRARY_COMMAND_POOL_HPP_
#define EE_LIBRARY_COMMAND_POOL_HPP_

#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include "EEForward.hpp"

namespace ee {
namespace detail {
class CommandPoolBase {
public:
    using ParamType = std::vector<std::string>;

    virtual ~CommandPoolBase() = default;

    virtual bool process(const ParamType& params) = 0;
};
} // namespace detail

template <class... Args> class CommandPool : public detail::CommandPoolBase {
private:
    using Self = CommandPool;
    using Super = detail::CommandPoolBase;

public:
    using Function = std::function<void(const Args&... args)>;
    using Placeholder = std::tuple<Args...>;
    using Command = std::pair<ParamType, Function>;

    static std::unique_ptr<Self> create() { return std::make_unique<Self>(); }

    virtual bool process(const ParamType& params) override {
        for (auto&& command : commands_) {
            if (process(params, command)) {
                return true;
            }
        }
        return false;
    }

    void addCommand(const ParamType& params, const Function& function) {
        commands_.emplace_back(params, function);
    }

private:
    bool process(const ParamType& params, const Command& command) const {
        auto&& expectedParams = command.first;
        if (expectedParams.size() != params.size()) {
            return false;
        }
        ParamType placeholderParams;
        for (std::size_t i = 0; i < params.size(); ++i) {
            if (expectedParams.at(i) == "%") {
                placeholderParams.push_back(params.at(i));
            } else if (expectedParams.at(i) != params.at(i)) {
                return false;
            }
        }

        Placeholder placeholders;
        try {
            parse(placeholderParams, placeholders);
        } catch (const std::exception& ex) {
            return false;
        }

        execute(command.second, placeholders);
        return true;
    }

    void execute(const Function& function,
                 const Placeholder& placeholders) const {
        execute(function, placeholders,
                std::make_index_sequence<sizeof...(Args)>());
    }

    template <std::size_t... Indices>
    void execute(const Function& function, const Placeholder& placeholders,
                 std::index_sequence<Indices...>) const {
        function(std::get<Indices>(placeholders)...);
    }

    void parse(const ParamType& params, Placeholder& placeholders) const {
        parse(params, placeholders,
              std::make_index_sequence<sizeof...(Args)>());
    }

    template <std::size_t... Indices>
    void parse(const ParamType& params, Placeholder& placeholders,
               std::index_sequence<Indices...>) const {
        // https://www.reddit.com/r/cpp/comments/2tffv3/for_each_argumentsean_parent
        [](...) {}(((std::get<Indices>(placeholders) =
                         ee::DataTraits<Args>::load(params.at(Indices))),
                    0)...);
    }

    std::vector<Command> commands_;
};

} // namespace ee

#endif /* EE_LIBRARY_COMMAND_POOL_HPP_ */
