#include "ee/core/ObserverHandle.hpp"

#include "ee/core/internal/ScopeGuard.hpp"

namespace ee {
namespace core {
using Self = ObserverHandle;

Self::ObserverHandle() = default;
Self::~ObserverHandle() = default;

void Self::add(const std::function<void()>& rollback) {
    guards_.emplace_back(rollback);
}

void Self::clear() {
    guards_.clear();
}
} // namespace core
} // namespace ee
