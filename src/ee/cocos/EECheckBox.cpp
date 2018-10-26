//
//  EECheckBox.cpp
//  roll-eat
//
//  Created by enrevol on 10/3/15.
//
//

#include <algorithm>

#include "EECheckBox.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
bool CheckBoxBase::isChecked() const noexcept {
    return isChecked_;
}

void CheckBoxBase::check() {
    setChecked(true);
}

void CheckBoxBase::uncheck() {
    setChecked(false);
}

void CheckBoxBase::setChecked(bool checked) {
    if (isChecked_ != checked) {
        isChecked_ = checked;
        onStateChanged(checked);
        if (onStateChangedCallback_) {
            onStateChangedCallback_(checked);
        }
    }
}

void CheckBoxBase::setOnStateChangedCallback(
    const StateChangedCallback& callback) {
    onStateChangedCallback_ = callback;
}
NS_DETAIL_END

void CheckAllBox::addCheckBox(CheckBox* checkBox) {
    checkBoxes_.push_back(checkBox);
    checkBox->checkAllBox_ = this;
}

const std::vector<CheckBox*>& CheckAllBox::getCheckBoxes() const {
    return checkBoxes_;
}

void CheckAllBox::clearAllCheckBoxes() {
    checkBoxes_.clear();
}

void CheckAllBox::setChecked(bool checked) {
    CheckBoxBase::setChecked(checked);
    for (auto box : checkBoxes_) {
        box->setChecked(checked);
    }
}

bool CheckAllBox::all() const {
    return std::all_of(checkBoxes_.cbegin(), checkBoxes_.cend(),
                       [](CheckBox* box) { return box->isChecked(); });
}

bool CheckAllBox::any() const {
    return std::any_of(checkBoxes_.cbegin(), checkBoxes_.cend(),
                       [](CheckBox* box) { return box->isChecked(); });
}

bool CheckAllBox::none() const {
    return std::none_of(checkBoxes_.cbegin(), checkBoxes_.cend(),
                        [](CheckBox* box) { return box->isChecked(); });
}

void CheckAllBox::updateState() {
    bool newState = std::all_of(checkBoxes_.cbegin(), checkBoxes_.cend(),
                                [](CheckBox* box) { return box->isChecked(); });
    if (newState != isChecked()) {
        CheckBoxBase::setChecked(newState);
    }
}

void CheckBox::setChecked(bool checked) {
    CheckBoxBase::setChecked(checked);

    if (checkAllBox_ != nullptr) {
        checkAllBox_->updateState();
    }
}
NS_EE_END