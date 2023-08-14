//
// Created by Nhan on 14/08/2023.
//

#ifndef COCOS2DCPP_INTERNALCLOUDDATAHANDLER_H
#define COCOS2DCPP_INTERNALCLOUDDATAHANDLER_H

#include <map>
#include "ee/play/ICloudDataHandler.h"

namespace ee::play {

class InternalCloudDataHandler : ee::play::ICloudDataHandler {
public:
    std::map<std::string, std::string> exportData() override;

    void importData(std::map<std::string, std::string> importData) override;

    void importData(const std::string &jsonData);

private:
    std::map<std::string, std::string> _data = {};
};

}

#endif //COCOS2DCPP_INTERNALCLOUDDATAHANDLER_H
