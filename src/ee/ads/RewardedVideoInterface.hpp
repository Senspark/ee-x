//
//  RewardedVideoInterface.hpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#ifndef EE_X_REWARDED_VIDEO_INTERFACE_HPP
#define EE_X_REWARDED_VIDEO_INTERFACE_HPP

#include <functional>
#include <string>

namespace ee {
namespace ads {
enum class RewardedVideoResult {
    Error,
    Skipped,
    Finished,
};

using RewardedVideoCallback = std::function<void(
    RewardedVideoResult result, const std::string& placementId)>;

class RewardedVideoInterface {
public:
    RewardedVideoInterface();
    virtual ~RewardedVideoInterface();

    /// Checks whether the rewarded video is available for displaying.
    /// @param[in] placementId The placement ID.
    virtual bool isRewardedVideoReady(const std::string& placementId) const = 0;

    /// Attempts to show the rewarded video.
    /// @param[in] placementId The placement ID.
    virtual bool showRewardedVideo(const std::string& placementId) = 0;

    void setRewardedVideoCallback(const RewardedVideoCallback& callback);

protected:
    void invokeRewardedVideoCallback(RewardedVideoResult result,
                                     const std::string& placementId);

private:
    RewardedVideoCallback callback_;
};
} // namespace ads

using ads::RewardedVideoResult;
using ads::RewardedVideoInterface;
} // namespace ee

#endif /* EE_X_REWARDED_VIDEO_INTERFACE_HPP */
