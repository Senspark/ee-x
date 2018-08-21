#ifdef COCOS2D_JAVASCRIPT

#include "JSBinding.h"
#include "CCError.h"
#include "jansson.h"
#include "CCJsonHelper.h"
#include "CCNdkBridge.h"

USING_NS_CC;

void Soomla::JSBinding::callNative(const char *params, std::string &result) {
    result.assign(params);

    CCLog("callNative: in >> %s", params);

    json_error_t error;
    json_t *root;
    root = json_loads(params, 0, &error);

    if (!root) {
        CCLog("error: at line #%d: %s", error.line, error.text);
        return;
    }

    cocos2d::CCObject *dataToPass = CCJsonHelper::getCCObjectFromJson(root);
    CCDictionary *dictToPass = dynamic_cast<CCDictionary *>(dataToPass);
    CC_ASSERT(dictToPass);

    soomla::CCError *soomlaError = NULL;
    CCDictionary *retParams = (CCDictionary *) soomla::CCNdkBridge::callNative(dictToPass, &soomlaError);

    CCDictionary *resultParams = CCDictionary::create();
    if (soomlaError != NULL) {
        retParams = CCDictionary::create();
        retParams->setObject(CCString::create(soomlaError->getInfo()), "info");

        resultParams->setObject(CCBool::create(false), "success");
    } else {
        resultParams->setObject(CCBool::create(true), "success");
    }
    resultParams->setObject(retParams, "result");

    root = CCJsonHelper::getJsonFromCCObject(resultParams);
    char *dump = json_dumps(root, JSON_COMPACT | JSON_ENSURE_ASCII);
    CCLog("callNative: out >> %s", dump);
    result = dump;
    free(dump);
}

void Soomla::JSBinding::callCallback(CCDictionary *params) {
    json_t *root = CCJsonHelper::getJsonFromCCObject(params);
    char *dump = json_dumps(root, JSON_COMPACT | JSON_ENSURE_ASCII);
    CCLog("ndkCallback: in >> %s", dump);

    std::string jsonParams = dump;
    free(dump);

    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();

    jsval v = std_string_to_jsval(cx, jsonParams);
    jsval retval;
    ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(ScriptingCore::getInstance()->getGlobalObject()),
            "ndkCallback", 1, &v, &retval);
}

#endif // COCOS2D_JAVASCRIPT