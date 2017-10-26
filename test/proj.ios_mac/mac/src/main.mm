#import <ee/Core.hpp>

#include "AppDelegate.hpp"

int main(int argc, char* argv[]) {
    eetest::AppDelegate app;
    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];
    return cocos2d::Application::getInstance()->run();
}
