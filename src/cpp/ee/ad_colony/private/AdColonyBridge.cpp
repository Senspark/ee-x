#include "ee/ad_colony/private/AdColonyBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace ad_colony {
namespace {
const std::string kPrefix = "AdColonyBridge";
const std::string kInitialize = kPrefix + "Initialize";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    destroyer_();
}

Task<bool> Self::initialize(const std::string& appId,
                            const std::vector<std::string>& zoneIds) {
    nlohmann::json json;
    json["appId"] = appId;
    json["zoneIds"] = zoneIds;
    auto response = co_await bridge_.callAsync(kInitialize, json.dump());
    co_return core::toBool(response);
}
} // namespace ad_colony
} // namespace ee
