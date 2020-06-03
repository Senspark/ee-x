#import <ee/Core.hpp>

#include "AppDelegate.hpp"

int main(int argc, char* argv[]) {
    eetest::AppDelegate app;
    EEPluginManager* manager = [EEPluginManager getInstance];
    [manager initializePlugins];
    return cocos2d::Application::getInstance()->run();
}
