//
//  RewardedAdSceneTester.hpp
//  ee_x_test_mobile
//
//  Created by eps on 10/7/20.
//

#ifndef EE_X_TEST_REWARDED_AD_SCENE_TESTER_HPP
#define EE_X_TEST_REWARDED_AD_SCENE_TESTER_HPP

#include <memory>

#include <ee/CppFwd.hpp>

#include "ITester.hpp"

namespace eetest {
class RewardedAdSceneTester : public ITester {
public:
    explicit RewardedAdSceneTester();

    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IAdMob> adMob_;
    std::shared_ptr<ee::IFacebookAds> facebookAds_;
    std::shared_ptr<ee::IIronSource> ironSource_;
    std::shared_ptr<ee::IUnityAds> unityAds_;
    std::shared_ptr<ee::IRewardedAd> rewardedAd_;

    std::unique_ptr<ee::ObserverHandle> handle_;
    cocos2d::Scene* scene_;
};
} // namespace eetest

#endif /* EE_X_TEST_REWARDED_AD_SCENE_TESTER_HPP */
