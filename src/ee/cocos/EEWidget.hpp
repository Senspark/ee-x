//
//  EEWidget.hpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#ifndef EE_LIBRARY_WIDGET_HPP
#define EE_LIBRARY_WIDGET_HPP

#include <platform/CCPlatformConfig.h> // CC_DLL

#include <cocosbuilder/CCNodeLoaderListener.h>
#include <ui/UIWidget.h>

#include "EEForward.hpp"

namespace ee {
namespace ui {
class Widget : public cocos2d::ui::Widget,
               public cocosbuilder::NodeLoaderListener {
private:
    using Self = Widget;
    using Super = cocos2d::ui::Widget;

public:
    static Self* create();

    virtual void addChild(Node* child, int localZOrder, int tag) override;

    virtual void addChild(Node* child, int localZOrder,
                          const std::string& name) override;

    using Super::addChild;

    virtual cocos2d::Node* getChildByTag(int tag) const override;

    virtual cocos2d::Node*
    getChildByName(const std::string& name) const override;

    virtual cocos2d::Vector<cocos2d::Node*>& getChildren() override;

    virtual const cocos2d::Vector<cocos2d::Node*>& getChildren() const override;

    virtual ssize_t getChildrenCount() const override;

    virtual void removeChild(cocos2d::Node* child,
                             bool cleanup = true) override;

    virtual void removeAllChildrenWithCleanup(bool cleanup = true) override;

    virtual void reorderChild(cocos2d::Node* child, int localZOrder) override;

    virtual void sortAllChildren() override;

    virtual void setCascadeColorEnabled(bool enabled) override;

    virtual void setCascadeOpacityEnabled(bool enabled) override;

    virtual const cocos2d::Size& getContentSize() const override;

    void setInset(float inset);
    void setInsetLeft(float inset);
    void setInsetTop(float inset);
    void setInsetRight(float inset);
    void setInsetBottom(float inset);

    const cocos2d::Size& getInnerContentSize() const;

protected:
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void onNodeLoaded(cocos2d::Node* node,
                              cocosbuilder::NodeLoader* nodeLoader) override;

    virtual void onSizeChanged() override;
    void updateInset();

private:
    friend WidgetLoader;

    bool magicEnabled_;

    float insetLeft_;
    float insetTop_;
    float insetRight_;
    float insetBottom_;

    cocos2d::ui::Widget* container_;

#ifndef NDEBUG
    cocos2d::DrawNode* drawNode_;
#endif // NDEBUG
};
} // namespace ui
} // namespace ee

#endif /* EE_LIBRARY_WIDGET_HPP */
