#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include <functional>
#include <string>
#include <vector>

namespace ee {
namespace vungle {
enum class VungleAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class Vungle final {
public:
    using AdCallback = std::function<void(VungleAdsResultCode code,
                                          const std::string& message)>;

    Vungle();
    ~Vungle();

    void initialize(const std::string& gameId,
                    const std::vector<std::string>& placementIds);

    bool isAdsReady(const std::string& placementId);

    void showAds(const std::string& placementId);

    void setCallback(const AdCallback& callback) { callback_ = callback; }

private:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
