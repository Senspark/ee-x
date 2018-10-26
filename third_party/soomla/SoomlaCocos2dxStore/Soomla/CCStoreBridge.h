

#ifndef __CCStoreBridge_H_
#define __CCStoreBridge_H_

#include "CCStoreAssets.h"
#include "cocos2d.h"

namespace soomla {

class CCStoreBridge : public cocos2d::Ref {
public:
    /**
    This class is singleton, use this function to access it.
    */
    static CCStoreBridge* getInstance();

    /**
    Initializes StoreBridge on native side and allows using its
    functions.
    @param storeParams A CCDictionary containing parameters for
    CCSoomlaStore (These were previously found in CCSoomla).
    This dictionary can contain the following:
    "androidPublicKey": __String - Your Android public key.
     "testPurchases": __Bool - Should test purchases be used in Android.
    "SSV": __Bool - Whether or not to enable server side
    verification of purchases.
    */
    static void initShared();

    virtual bool init();
    
    virtual void applyParams(const cocos2d::ValueMap& storeParams);
};
} // namespace soomla

#endif //__CCStoreBridge_H_
