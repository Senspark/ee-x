//
//  CrashlyticsAgent.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_CRASHLYTICS_AGENT_HPP_
#define EE_X_TEST_CRASHLYTICS_AGENT_HPP_

#include <memory> // std::unique_ptr.
#include <string> // std::string.

#include <ee/FirebaseFwd.hpp>

class CrashlyticsAgent {
public:
    static CrashlyticsAgent* getInstance();

    void initialize();
    
    void log(const std::string& message);
    void logDebug(const std::string& message);
    void logInfo(const std::string& message);

private:
    std::unique_ptr<ee::FirebaseCrashlytics> plugin_;
};

#endif /* EE_X_TEST_CRASHLYTICS_AGENT_HPP_ */
