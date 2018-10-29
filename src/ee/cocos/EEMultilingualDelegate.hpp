//
//  EEMultilingualDelegate.hpp
//  ee-library
//
//  Created by Zinge on 7/31/17.
//
//

#ifndef EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP
#define EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP

#include "EECocos2dxFwd.hpp"
#include "EEForward.hpp"
#include "EELanguageDelegate.hpp"

#include <platform/CCPlatformDefine.h> // CC_DLL

#include <cocosbuilder/CCBMemberVariableAssigner.h>
#include <cocosbuilder/CCNodeLoaderListener.h>

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

#endif /* EE_LIBRARY_MULTILINGUAL_DELEGATE_HPP */
