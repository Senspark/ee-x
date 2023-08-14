//
// Created by Nhan on 14/08/2023.
//

#ifndef COCOS2DCPP_CLOUDDATA_H
#define COCOS2DCPP_CLOUDDATA_H

#include <ee/nlohmann/json.hpp>
#include <memory>

namespace ee::play {

class CloudData {
public:
    std::string originData;
    std::string desc;
    std::string hash;
    long timeStamp;

    static std::unique_ptr<CloudData> parseJson(const std::string& json);

    std::string toJson();
};

}


#endif //COCOS2DCPP_CLOUDDATA_H