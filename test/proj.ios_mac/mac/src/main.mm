#import <ee/Core.hpp>

#include "AppDelegate.hpp"

int main(int argc, char* argv[]) {
    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];

    // Simulate OSX.
    AppDelegate::getInstance()->applicationDidFinishLaunching();
    return 0;
}
