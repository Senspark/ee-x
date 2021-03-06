#ifndef EE_X_I_SERVICE_HPP
#define EE_X_I_SERVICE_HPP

#ifdef __cplusplus

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class IService {
public:
    virtual ~IService() = default;

    /// Initializes this service.
    virtual Task<bool> initialize() = 0;

    /// Destroys this service.
    virtual void destroy() = 0;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_SERVICE_HPP
