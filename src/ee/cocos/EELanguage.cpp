//
//  EELanguage.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguage.hpp"

namespace ee {
namespace language {
using Self = Language;

const Self Self::English("en");
const Self Self::Thailand("th");
const Self Self::Vietnamese("vi");
const Self Self::Russian("ru");

Self Self::create(const std::string& code) {
    return Self(code);
}

Self::Language(const std::string& code)
    : code_(code) {}

const std::string& Self::getCode() const {
    return code_;
}

bool Self::operator==(const Self& other) const {
    return code_ == other.code_;
}
} // namespace language
} // namespace ee
