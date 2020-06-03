
#ifndef __CCVirtualItemReward_H_
#define __CCVirtualItemReward_H_

#ifdef __cplusplus

#include <soomla/CCCoreConsts.h>
#include <soomla/rewards/CCReward.h>

#include "soomla/CCStoreConsts.h"

namespace soomla {
/**
 A specific type of `CCReward` is the one you'll use to give your
 users some amount of a virtual item when they complete something.
 */
class CCVirtualItemReward : public CCReward {
public:
    /**
     Creates a `CCVirtualItemReward`.
     @param rewardId See parent
     @param name See parent
     @param amount The amount of associated items to give when reward is
     given
     @param associatedItemId The ID of the virtual items to give
     @return The Virtual Item Reward.
     */
    static CCVirtualItemReward* create(const std::string& rewardId,
                                       const std::string& name,
                                       std::int32_t amount,
                                       const std::string& associatedItemId);
    /**
     Creates a `CCVirtualItemReward` from a dictionary.
     @param dict A dictionary containing keys for the reward's properties
     @return The Virtual Item Reward.
     */
    SL_CREATE_WITH_VALUE_MAP(CCVirtualItemReward);
    
    using CCReward::init;

    virtual bool init(const std::string& rewardId, const std::string& name,
                      const std::string& associatedItemId, std::int32_t amount);

    virtual bool initWithValueMap(const cocos2d::ValueMap& dict) override;
    /**
     Converts the reward into a `CCDictionary`.
     @return `CCDictionary` representation of this `CCVirtualItemReward`.
     */
    virtual cocos2d::ValueMap toValueMap() override;

    virtual ~CCVirtualItemReward() = default;

    virtual const char* getType() const override;

    CC_SYNTHESIZE(std::int32_t, mAmount, Amount);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mAssociatedItemId, AssociatedItemId);

protected:
    virtual bool takeInner() override;

    virtual bool giveInner() override;
    
    void fillAmountFromValueMap(const cocos2d::ValueMap& map);
    void putAmountToValueMap(cocos2d::ValueMap& map);
    void fillAssociatedItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putAssociatedItemIdToValueMap(cocos2d::ValueMap& map);
};
}; // namespace soomla

#endif // __cplusplus

#endif //__CCVirtualItemReward_H_
