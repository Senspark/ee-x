//
// Created by Nhan on 14/08/2023.
//

#include "InternalCloudDataHandler.h"
#include <ee/nlohmann/json.hpp>

namespace ee::play {
std::map<std::string, std::string> InternalCloudDataHandler::exportData() {
    return _data;
}

void InternalCloudDataHandler::importData(std::map<std::string, std::string> importData) {
    _data = importData;
}

void InternalCloudDataHandler::importData(const std::string &jsonData) {
    _data = {};
    try {
        nlohmann::json j = nlohmann::json::parse(jsonData);
        for (auto &it: j.items()) {
            _data[it.key()] = it.value();
        }
    } catch (std::exception &e) {
        // do nothing
    }
}

} // namespace