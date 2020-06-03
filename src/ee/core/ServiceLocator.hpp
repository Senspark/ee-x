#ifndef EE_X_SERVICE_LOCATOR_HPP
#define EE_X_SERVICE_LOCATOR_HPP

#ifdef __cplusplus

#include <memory>
#include <string>
#include <unordered_set>

#include "ee/core/IService.hpp"

namespace ee {
namespace core {
namespace detail {
class ServiceLocatorImpl {
private:
    template <class Service>
    static constexpr bool IsService = std::is_base_of_v<IService, Service>;

public:
    template <class Service, class T,
              class = std::enable_if_t<IsService<Service>>>
    void provide(const std::shared_ptr<T>& service) {
        auto&& currentService = getService<Service>();
        if (currentService) {
            currentService->destroy();
            services_.erase(currentService.get());
        }
        currentService = service;
        services_.insert(currentService.get());
    }

    template <class Service, class = std::enable_if_t<IsService<Service>>>
    const std::shared_ptr<Service>& resolve() {
        auto&& service = getService<Service>();
        return service;
    }

private:
    template <class Service>
    std::shared_ptr<Service>& getService() {
        static std::shared_ptr<Service> service;
        return service;
    }

    std::unordered_set<IService*> services_;
};
} // namespace detail

class ServiceLocator {
public:
    /// Registers a service.
    /// @note register is a keyword.
    template <class Service, class T>
    static void provide(const std::shared_ptr<T>& service) {
        impl_.provide<Service>(service);
    }

    /// Resolves the specified service.
    template <class Service>
    static const std::shared_ptr<Service>& resolve() {
        return impl_.resolve<Service>();
    }

private:
    static detail::ServiceLocatorImpl impl_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_SERVICE_LOCATOR_HPP
