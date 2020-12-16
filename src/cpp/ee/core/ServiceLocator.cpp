#include "ee/core/ServiceLocator.hpp"

namespace ee {
namespace core {
using Self = ServiceLocator;

detail::ServiceLocatorImpl Self::impl_;
} // namespace core
} // namespace ee
