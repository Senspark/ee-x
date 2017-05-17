#import <ee/Core.hpp>

#include "CrashlyticsAgent.hpp"

int main(int argc, char* argv[]) {
    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];

    CrashlyticsAgent::getInstance()->initialize();
    CrashlyticsAgent::getInstance()->logDebug("debug_message");
    CrashlyticsAgent::getInstance()->logInfo("info_message");
    CrashlyticsAgent::getInstance()->logError("error_message");

    return 0;
}
