//
//  EEConsole.hpp
//  ee-library
//
//  Created by Zinge on 4/10/17.
//
//

#ifndef EE_LIBRARY_CONSOLE_HPP_
#define EE_LIBRARY_CONSOLE_HPP_

#include "EECocos2dxFwd.hpp"

#include <ui/UIWidget.h>

namespace ee {
namespace detail {
class CommandPoolBase;
class ConsoleDelegate;
} // namespace detail

class Console : public cocos2d::ui::Widget {
private:
    using Self = Console;
    using Super = cocos2d::ui::Widget;

public:
    static Self* create();

    Self* addCommandPool(std::unique_ptr<detail::CommandPoolBase> pool);

protected:
    Console();
    virtual ~Console() override;

    virtual bool init() override;

    virtual bool onTouchBegan(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

    virtual void onTouchMoved(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

    virtual void onTouchEnded(cocos2d::Touch* touch,
                              cocos2d::Event* event) override;

private:
    friend detail::ConsoleDelegate;

    std::vector<std::unique_ptr<detail::CommandPoolBase>> pools_;

    cocos2d::ui::EditBox* editBox_;
    std::unique_ptr<cocos2d::ui::EditBoxDelegate> editBoxDelegate_;
};
} // namespace ee

#endif /* EE_LIBRARY_CONSOLE_HPP_ */
