#ifndef EE_X_I_SERVICE_HPP
#define EE_X_I_SERVICE_HPP

#include <ee/CoreFwd.hpp>

namespace ee {
namespace core {
class IService {
public:
    virtual ~IService() = default;

    /// Initializes this service.
    virtual void initialize() = 0;

    /// Destroys this service.
    virtual void destroy() = 0;
};
} // namespace core
} // namespace ee

#endif // EE_X_I_SERVICE_HPP