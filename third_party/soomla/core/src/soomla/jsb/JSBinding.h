

#ifndef __JSBinding_H_
#define __JSBinding_H_

#ifdef COCOS2D_JAVASCRIPT

#include "cocos2d.h"
#include "ScriptingCore.h"

// Define a namespace to manage your code and make your code clearly
namespace Soomla {
    class JSBinding: public cocos2d::CCObject
    {
    public:
        static void callNative(const char *params, std::string &result);
        static void callCallback(cocos2d::CCDictionary *params);
    };
}

#endif //COCOS2D_JAVASCRIPT

#endif //__JSBinding_H_
