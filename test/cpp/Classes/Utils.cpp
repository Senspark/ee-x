//
//  Utils.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "Utils.hpp"

#include <sstream>

#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace eetest {
void schedule(float delay, float interval, unsigned times,
              const detail::Tick& f) {
    static int target;
    static int counter;
    auto director = cocos2d::Director::getInstance();
    auto scheduler = director->getScheduler();
    scheduler->schedule(std::bind(f), &target, interval, times, delay, false,
                        std::to_string(counter++));
}

void scheduleForever(float delay, float interval, const detail::Tick& f) {
    schedule(delay, interval, CC_REPEAT_FOREVER, f);
}

void scheduleOnce(float delay, const detail::Tick& f) {
    schedule(delay, 0, 0, f);
}

ee::Logger& getLogger() {
    static ee::Logger logger("ee_x");
    return logger;
}

std::string getCurrentThreadId() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

void logCurrentThread() {
    getLogger().info("Current thread ID: %s", getCurrentThreadId().c_str());
}
} // namespace eetest
