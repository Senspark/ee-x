#ifndef EE_X_VUNGLE_SRC_BRIDGE_HPP
#define EE_X_VUNGLE_SRC_BRIDGE_HPP

#include <memory>
#include <string>
#include <functional>

namespace ee {
namespace vungle {
enum class VungleAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class VungleAds final {
public:
    using AdCallback = std::function<void(VungleAdsResultCode code,
                                          const std::string& message)>;

    VungleAds();
    ~VungleAds();

    void initVungleAds(const std::string& gameId, const std::string& placementIds);

    bool isAdsReady(const std::string& placementId);

    void showAds(const std::string& placementId);

    void setCallback(const AdCallback& callback) { callback_ = callback; }

private:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // VungleAds
} // namespace ee

#endif /* EE_X_VUNGLE_SRC_BRIDGE_HPP */
