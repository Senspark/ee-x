//
//  EEConsoleDelegate.hpp
//  ee-library
//
//  Created by Zinge on 4/10/17.
//
//

#ifndef EE_LIBRARY_CONSOLE_DELEGATE_HPP_
#define EE_LIBRARY_CONSOLE_DELEGATE_HPP_

#ifdef __cplusplus

#include <ui/UIEditBox/UIEditBox.h>

#include <ee/CocosFwd.hpp>

namespace ee {
namespace detail {
class ConsoleDelegate : public cocos2d::ui::EditBoxDelegate {
public:
    explicit ConsoleDelegate(Console* console);

    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;

    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;

    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox,
                                    const std::string& text) override;

    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

private:
    Console* console_;
};
} // namespace detail
} // namespace ee

#endif // __cplusplus

#endif /* EE_LIBRARY_CONSOLE_DELEGATE_HPP_ */
