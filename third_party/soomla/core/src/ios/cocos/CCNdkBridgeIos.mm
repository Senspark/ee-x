

#include "soomla/CCJsonHelper.h"
#include "CCNdkBridgeIos.h"
#include "CCSoomlaEventDispatcher.h"
#include "NdkGlue.h"

namespace soomla {
json_t* CCNdkBridgeIos::receiveCppMessage(json_t* jsonParams) {
    char* pszJsonParams =
        json_dumps(jsonParams, JSON_COMPACT | JSON_ENSURE_ASCII);
    NSString* jsonParamsStr =
        [[NSString alloc] initWithUTF8String:pszJsonParams];
    free(pszJsonParams);

    NSData* jsonParamsData =
        [jsonParamsStr dataUsingEncoding:NSUTF8StringEncoding];

    // parse out the json data
    NSError* error = nil;
    NSDictionary* dictParams =
        [NSJSONSerialization JSONObjectWithData:jsonParamsData
                                        options:kNilOptions
                                          error:&error];

    if (error == nil) {
        NSObject* retParamsNs =
            [[NdkGlue sharedInstance] dispatchNdkCall:dictParams];

        if (retParamsNs != nil) {
            error = nil;
            NSData* retJsonParamsData = [NSJSONSerialization
                dataWithJSONObject:retParamsNs
                           options:NSJSONWritingPrettyPrinted
                             error:&error];

            if (error == nil) {
                NSString* retJsonParamsStr =
                    [[NSString alloc] initWithData:retJsonParamsData
                                          encoding:NSUTF8StringEncoding];

                json_error_t jerror;
                json_t* retJsonParams =
                    json_loads([retJsonParamsStr UTF8String], 0, &jerror);

                if (retJsonParams) {
                    return retJsonParams;
                } else {
                    fprintf(stderr, "error: at line #%d: %s\n", jerror.line,
                            jerror.text);
                    return nil;
                }
            } else {
                return nil;
            }

        } else {
            return nil;
        }
    } else {
        return nil;
    }
}

void CCNdkBridgeIos::ndkCallback(json_t* jsonParams) {
    auto map = CCJsonHelper::getValueFromJson(jsonParams).asValueMap();
    CCSoomlaEventDispatcher::getInstance()->ndkCallback(map);
}
} // namespace soomla
