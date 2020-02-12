#ifndef EE_X_SERVICE_LOCATOR_HPP
#define EE_X_SERVICE_LOCATOR_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include "ee/CoreFwd.hpp"
#include "ee/core/IService.hpp"

namespace ee {
namespace core {
namespace detail {
class ServiceLocatorImpl {
public:
    template <class Service, class T>
    void provide(std::unique_ptr<T> service) {
        auto&& currentService = getService<Service>();
        if (currentService) {
            currentService->destroy();
            services_.erase(currentService.get());
        }
        currentService = std::move(service);
        services_.insert(currentService.get());
    }

    template <class Service>
    const std::unique_ptr<Service>& resolve() {
        auto&& service = getService<Service>();
        return service;
    }

private:
    template <class Service>
    std::unique_ptr<Service>& getService() {
        static std::unique_ptr<Service> service;
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
    static void provide(std::unique_ptr<T> service) {
        impl_.provide<Service>(std::move(service));
    }

    /// Resolves the specified service.
    template <class Service>
    static const std::unique_ptr<Service>& resolve() {
        return impl_.resolve<Service>();
    }

private:
    static detail::ServiceLocatorImpl impl_;
};
} // namespace core
} // namespace ee

#endif // EE_X_SERVICE_LOCATOR_HPP
