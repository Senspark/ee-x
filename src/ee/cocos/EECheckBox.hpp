//
//  EECheckBox.h
//  roll-eat
//
//  Created by enrevol on 10/3/15.
//
//

#ifndef EE_LIBRARY_CHECK_BOX_HPP_
#define EE_LIBRARY_CHECK_BOX_HPP_

#include <functional>
#include <vector>

#include "EEMacro.hpp"

NS_EE_BEGIN
class CheckBox;

NS_DETAIL_BEGIN
class CheckBoxBase {
public:
    using StateChangedCallback = std::function<void(bool checked)>;

    virtual ~CheckBoxBase() = default;

    bool isChecked() const noexcept;

    virtual void setChecked(bool checked);

    void uncheck();
    void check();

    void setOnStateChangedCallback(const StateChangedCallback& callback);

protected:
    virtual void onStateChanged(bool checked) = 0;

private:
    bool isChecked_;

    StateChangedCallback onStateChangedCallback_;
};
NS_DETAIL_END

class CheckAllBox : public detail::CheckBoxBase {
public:
    void addCheckBox(CheckBox* checkBox);

    void setCheckBox(const std::vector<CheckBox>& checkBoxes);

    const std::vector<CheckBox*>& getCheckBoxes() const;

    void clearAllCheckBoxes();

    bool all() const;

    bool any() const;

    bool none() const;

    virtual void setChecked(bool checked) override;

protected:
    friend class CheckBox;

    virtual void updateState();

private:
    std::vector<CheckBox*> checkBoxes_;
};

class CheckBox : public detail::CheckBoxBase {
public:
    virtual void setChecked(bool checked) override;

private:
    friend class CheckAllBox;

    CheckAllBox* checkAllBox_;
};
NS_EE_END

#endif /* EE_LIBRARY_CHECK_BOX_HPP_ */
