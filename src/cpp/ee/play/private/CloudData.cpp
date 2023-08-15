//
// Created by Nhan on 14/08/2023.
//

#include "CloudData.h"

namespace ee::play {

std::unique_ptr<CloudData> CloudData::parseJson(const std::string &json) {
    try {
        nlohmann::json j = nlohmann::json::parse(json);
        auto data = std::make_unique<CloudData>();
        data->originData = j["originData"];
        data->desc = j["desc"];
        data->hash = j["hash"];
        data->timeStamp = j["timeStamp"];
        return data;
    } catch (std::exception &e) {
        return nullptr;
    }
}

std::string CloudData::toJson() {
    nlohmann::json j;
    j["originData"] = originData;
    j["desc"] = desc;
    j["hash"] = hash;
    j["timeStamp"] = timeStamp;
    return j.dump();
}

} // play
// ee