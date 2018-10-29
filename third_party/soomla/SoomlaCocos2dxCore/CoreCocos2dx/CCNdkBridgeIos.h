
#ifndef __CCProfileNdkBridgeIos_H_
#define __CCProfileNdkBridgeIos_H_

#include <string>
#include "jansson.h"

namespace soomla {
    class CCNdkBridgeIos {
    public:
        static json_t *receiveCppMessage(json_t *jsonParams);
        static void ndkCallback(json_t *jsonParams);
    };
};

#endif //__CCProfileNdkBridgeIos_H_
