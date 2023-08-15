//
// Created by Nhan on 14/08/2023.
//

#ifndef COCOS2DCPP_LIBRARIES_EE_X_SRC_CPP_EE_PLAY_ICLOUDDATAHANDLER_H
#define COCOS2DCPP_LIBRARIES_EE_X_SRC_CPP_EE_PLAY_ICLOUDDATAHANDLER_H

#include <map>
#include <string>

namespace ee::play {

class ICloudDataHandler {
public:
    virtual ~ICloudDataHandler() {};
    virtual std::map<std::string, std::string> exportData() = 0;

    virtual void importData(std::map<std::string, std::string> importData) = 0;
};

} // ee

#endif //COCOS2DCPP_LIBRARIES_EE_X_SRC_CPP_EE_PLAY_ICLOUDDATAHANDLER_H
