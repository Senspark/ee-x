//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_RECORDER_BRIDGE_HPP
#define EE_X_RECORDER_BRIDGE_HPP

#include <string>

#include "ee/RecorderFwd.hpp"

namespace ee {
namespace recorder {
class Recorder final {
public:
    Recorder();
    ~Recorder();

    void startRecording();
    void stopRecording();
    void cancelRecording();

    std::string getRecordingUrl() const;
    bool checkRecordingPermission() const;

private:
    MessageBridge& bridge_;
};
} // namespace recorder
} // namespace ee

#endif /* EE_X_RECORDER_BRIDGE_HPP */
