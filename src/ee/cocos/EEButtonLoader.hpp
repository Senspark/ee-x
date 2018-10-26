//
//  EEButtonLoader.hpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#ifndef EE_LIBRARY_BUTTON_LOADER_HPP_
#define EE_LIBRARY_BUTTON_LOADER_HPP_

#include "EEUiButtonLoader.hpp"

namespace ee {
class ButtonLoader : public UiButtonLoader {
private:
    using Self = ButtonLoader;
    using Super = UiButtonLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Self, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloat(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, float floatValue,
                          cocosbuilder::CCBReader* reader) override;
};
} // namespace ee

#endif /* EE_LIBRARY_BUTTON_LOADER_HPP_ */
