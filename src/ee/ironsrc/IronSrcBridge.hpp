#ifndef EE_X_IRON_SRC_BRIDGE_HPP
#define EE_X_IRON_SRC_BRIDGE_HPP

#include <memory>
#include <string>
#include <functional>

namespace ee {
namespace ironsrc {
enum class IronSrcAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class IronSrc final {
public:
    using AdCallback = std::function<void(IronSrcAdsResultCode code,
                                          const std::string& message)>;

    IronSrc();
    ~IronSrc();

    void initIronSrc(const std::string& gameId);

    bool isAdsReady(const std::string& placementId);

    void showAds(const std::string& placementId);

    void setCallback(const AdCallback& callback) { callback_ = callback; }

private:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // ironsrc
} // namespace ee

#endif /* EE_X_IRON_SRC_BRIDGE_HPP */
