//
//  EEMultilingualDelegate.hpp
//  ee-library
//
//  Created by Zinge on 7/31/17.
//
//

#ifndef EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP
#define EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP

#ifdef __cplusplus

// Must included first.
#include "ee/cocos/CocosFwd.hpp"

#include <cocosbuilder/CCBMemberVariableAssigner.h>
#include <cocosbuilder/CCNodeLoaderListener.h>

#include "ee/cocos/EELanguageDelegate.hpp"

namespace ee {
/// = LanguageDelegate + CocosBuilder.
class MultilingualDelegate : public LanguageDelegate,
                             public cocosbuilder::NodeLoaderListener,
                             public cocosbuilder::CCBMemberVariableAssigner {
private:
    using Self = MultilingualDelegate;
    using Super = LanguageDelegate;

public:
protected:
    virtual void onNodeLoaded(cocos2d::Node* node,
                              cocosbuilder::NodeLoader* nodeLoader) override;

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* target,
                                           const char* memberVariableName,
                                           cocos2d::Node* node) override;

    virtual bool
    onAssignCCBCustomProperty(cocos2d::Ref* target,
                              const char* memberVariableName,
                              const cocos2d::Value& value) override;
};
} // namespace ee

#endif // __cplusplus

#endif /* EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP */
