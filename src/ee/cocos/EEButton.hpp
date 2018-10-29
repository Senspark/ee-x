//
//  EEButton.hpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#ifndef EE_LIBRARY_BUTTON_HPP_
#define EE_LIBRARY_BUTTON_HPP_

#include <array>
#include <functional>

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEForward.hpp"

#include <ui/UIButton.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
class ButtonEx;
NS_DETAIL_END

using Button = detail::ButtonEx;

NS_DETAIL_BEGIN
/// Improved button class for @c cocos2d::ui::Button:
///
/// - Correct hit test.
///
/// - Children will be scaled when the button is pressed.
///
/// Use the following code to register the loader.
/// @code
/// your_node_loader_library->registerNodeLoader("class_name_in_cocosbuilder",
///                                              ee::GenericLoader<ee::Button,
///                                              ee::UiButtonLoader>::loader());
/// @endcode
class ButtonEx : public cocos2d::ui::Button {
private:
    using Self = ButtonEx;
    using Super = cocos2d::ui::Button;

public:
    using Loader = GenericLoader<Self, UiButtonLoader>;

    using HitTester =
        std::function<bool(cocos2d::Touch* touch, Button* button)>;

    using TouchCallback =
        std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)>;

    /// Retrieves the default inside checker.
    static const HitTester& getDefaultHitTester();

    /// Default constructor.
    ButtonEx();

    /// Default destructor.
    virtual ~ButtonEx() override;

    /// Creates an empty Button.
    /// @return An empty Button instance.
    static Self* create();

    /// Creates a button with custom textures.
    /// @param normalImage      normal state texture name.
    /// @param selectedImage    selected state texture name.
    /// @param disableImage     disabled state texture name.
    /// @param texType          texture resource type.
    /// @see @c cocos2d::ui::Button::TextureResType
    /// @return a Button instance.
    static Self* create(const std::string& normalImage,
                        const std::string& selectedImage = "",
                        const std::string& disableImage = "",
                        TextureResType texType = TextureResType::LOCAL);

    /// Adds a child to the internal container.
    /// @see @c cocos2d::Node::addChild.
    virtual void addChild(Node* child, int localZOrder, int tag) override;

    /// Adds a child to the internal container.
    /// @see @c cocos2d::Node::addChild.
    virtual void addChild(Node* child, int localZOrder,
                          const std::string& name) override;

    /// Inherits other @c cocos2d::Node::addChild overloads.
    using Super::addChild;

    /// @see @c cocos2d::Node::getChildByTag.
    virtual cocos2d::Node* getChildByTag(int tag) const override;

    /// @see @c cocos2d::Node::getChildByName.
    virtual cocos2d::Node*
    getChildByName(const std::string& name) const override;

    /// @see @c cocos2d::Node::getChildren.
    virtual cocos2d::Vector<cocos2d::Node*>& getChildren() override;

    /// @see @c cocos2d::Node::getChildren.
    virtual const cocos2d::Vector<cocos2d::Node*>& getChildren() const override;

    /// @see @c cocos2d::Node::getChildrenCount.
    virtual ssize_t getChildrenCount() const override;

    /// @see @c cocos2d::Node::removeChild.
    virtual void removeChild(cocos2d::Node* child,
                             bool cleanup = true) override;

    /// @see @c cocos2d::Node::removeAllChildrenWithCleanup.
    virtual void removeAllChildrenWithCleanup(bool cleanup = true) override;

    /// @see @c cocos2d::Node::reorderChild.
    virtual void reorderChild(cocos2d::Node* child, int localZOrder) override;

    /// @see @c cocos2d::Node::sortAllChildren.
    virtual void sortAllChildren() override;

    /// @see @c cocos2d::ui::Button::setContentSize.
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    virtual void setCascadeColorEnabled(bool enabled) override;

    virtual void setCascadeOpacityEnabled(bool enabled) override;

    /// @see @c cocos2d::ui::Widget::hitTest.
    virtual bool hitTest(const cocos2d::Point& point,
                         const cocos2d::Camera* camera,
                         cocos2d::Vec3* p) const override;

    virtual bool onTouchBegan(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

    virtual void onTouchMoved(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

    virtual void onTouchEnded(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

    /// @see @c cocos2d::ui::Button::setScale9Enabled.
    virtual void setScale9Enabled(bool enable) override;

    virtual std::string getDescription() const override;

    /// Assigns the zooming duration for the default animation.
    void setZoomingDuration(float duration) noexcept;

    /// Retrieves the zooming duration for the default animation.
    ///
    /// Default is 0.05f (seconds).
    float getZoomingDuration() const noexcept;

    void pressedStateBrightness(float brightness);

    void setTouchBeganCallback(const TouchCallback& callback);

    void setTouchMovedCallback(const TouchCallback& callback);

    void setTouchEndedCallback(const TouchCallback& callback);

    /// Retrieves the internal container.
    cocos2d::ui::Widget* getContainer() noexcept;

    /// Retrieves the internal container (@c const version).
    const cocos2d::ui::Widget* getContainer() const noexcept;

    Scale9SpriteWithHsv* getRendererNormal() const;

    Scale9SpriteWithHsv* getRendererClicked() const;

    Scale9SpriteWithHsv* getRendererDisabled() const;

    void setSaturation(float saturation);
    void setBrightness(float brightness);

protected:
    virtual bool init() override;

    virtual bool init(const std::string& normalImage,
                      const std::string& selectedImage,
                      const std::string& disableImage,
                      TextureResType texType) override;

    virtual void initRenderer() override;

    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;

    virtual void adaptRenderers() override;

    virtual Self* createCloneInstance() override;
    virtual void copySpecialProperties(cocos2d::ui::Widget* model) override;

    virtual void visit(cocos2d::Renderer* renderer,
                       const cocos2d::Mat4& parentTransform,
                       std::uint32_t parentFlags) override;

    /// Update texture for current state after loading new texture via
    /// @c loadTextures, @c loadTextureNormal, @c loadTexturePressed
    /// or @c loadTextureDisabled.
    virtual void updateTexture();

private:
    float saturation_;
    float brightness_;
    float zoomingDuration_;

    cocos2d::Node* zoomingAction_;

    cocos2d::Touch* currentTouch_;
    cocos2d::Event* currentEvent_;

    cocos2d::ui::Widget* container_;
};
NS_DETAIL_END
NS_EE_END

#endif /* EE_LIBRARY_BUTTON_HPP_ */
