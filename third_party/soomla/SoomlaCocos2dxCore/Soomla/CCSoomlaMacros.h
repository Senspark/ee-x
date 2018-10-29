/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __CCSoomlaMacros_h
#define __CCSoomlaMacros_h

#include "cocos2d.h"

#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
if (varName != var) \
{ \
CC_SAFE_RETAIN(var); \
CC_SAFE_RELEASE(varName); \
varName = var; \
} \
}

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define CC_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define CC_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define CC_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define CC_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define CC_BREAK_IF(cond)           if(cond) break

#define SL_SYNTHESIZE_RETAIN_WITH_DICT(varType, varName, funName, jsonKey)    \
CC_SYNTHESIZE_RETAIN(varType, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::__Dictionary* dict) \
{ \
    cocos2d::Ref* obj = dict->objectForKey(jsonKey); \
    CCASSERT(obj == nullptr || dynamic_cast<varType>(obj), "invalid object type in dictionary"); \
    if (varName != obj) \
    { \
        set##funName((varType)obj); \
    } \
} \
protected: inline void put##funName##ToDict(cocos2d::__Dictionary* dict) { \
  if (varName) { \
    dict->setObject(varName, jsonKey); \
  } \
}

#define SL_SYNTHESIZE_DOUBLE_RETAIN_WITH_DICT(varName, funName, jsonKey) \
CC_SYNTHESIZE_RETAIN(cocos2d::__Double *, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::__Dictionary* dict) \
{ \
    cocos2d::__Double *val = nullptr; \
    cocos2d::Ref* obj = dict->objectForKey(jsonKey); \
    if (obj != nullptr) { \
        val = dynamic_cast<cocos2d::__Double *>(obj); \
        if (val == nullptr) { \
            CCASSERT(dynamic_cast<cocos2d::__Integer *>(obj), "invalid object type in dictionary"); \
            val = cocos2d::__Double::create(((cocos2d::__Integer *)obj)->getValue()); \
        }\
    } \
    if (varName != obj) \
    { \
        set##funName((cocos2d::__Double *)obj); \
    } \
} \
protected: inline void put##funName##ToDict(cocos2d::__Dictionary* dict) { \
  if (varName) { \
    dict->setObject(varName, jsonKey); \
  } \
}

#define SL_SYNTHESIZE_RETAIN_WITH_DICT_DCL(varType, varName, funName)    \
CC_SYNTHESIZE_RETAIN(varType, varName, funName); \
protected: inline void fill##funName##FromDict(cocos2d::__Dictionary* dict); \
protected: inline void put##funName##ToDict(cocos2d::__Dictionary* dict); \

#define SL_CREATE_WITH_DICTIONARY(__class) \
static __class *createWithDictionary(cocos2d::__Dictionary *dict) { \
    __class *ret = new __class(); \
    if (ret->initWithDictionary(dict)) { \
        ret->autorelease(); \
    } \
    else { \
        CC_SAFE_DELETE(ret); \
    } \
    return ret; \
} \

#define SL_CREATE_WITH_VALUE_MAP(__class) \
static __class *createWithValueMap(const cocos2d::ValueMap& dict) { \
    __class *ret = new __class(); \
    if (ret->initWithValueMap(dict)) { \
        ret->autorelease(); \
    } \
    else { \
        CC_SAFE_DELETE(ret); \
    } \
    return ret; \
} \

#define SL_SAFE_CREATE_FROM_RETURN(__T__, __ret__, __retParams__)			\
    SL_SAFE_CREATE(__T__, __ret__, (__retParams__->objectForKey("return")))

#define SL_SAFE_CREATE(__T__, __ret__, __ref__)			\
    __T__ __ret__ = nullptr;\
    {\
        __Dictionary *dict = dynamic_cast<__Dictionary *>(__ref__); \
        CC_ASSERT(dict); \
        soomla::CCDomain *domain = CCDomainFactory::getInstance()->createWithDictionary(dict); \
	    __ret__ = dynamic_cast<__T__>(domain);			\
	    CC_ASSERT(__ret__); \
    }\

#define SL_EXTRACT_FROM_RETURN(__T__, __ret__, __retParams__) \
__T__ *__ret__ = nullptr; \
{ \
  Ref *retRef = __retParams__->objectForKey("return"); \
  CC_ASSERT(retRef); \
  __ret__ = dynamic_cast<__T__ *>(retRef); \
  CC_ASSERT(__ret__); \
} \

#define SL_EXTRACT_DOUBLE_FROM_RETURN(__ret__, __retParams__) \
__Double *__ret__ = nullptr; \
{ \
  Ref *retRef = __retParams__->objectForKey("return"); \
  CC_ASSERT(retRef); \
  __ret__ = dynamic_cast<__Double *>(retRef); \
  if (__ret__ == nullptr) { \
    __Integer *intRef = dynamic_cast<__Integer *>(retRef); \
    if (intRef != nullptr) { \
        __ret__ = __Double::create(intRef->getValue());\
    }\
  } \
  CC_ASSERT(__ret__); \
} \

#define SL_CREATE_PARAMS_FOR_METHOD(__ret__, __methodName__) \
__Dictionary *__ret__ = __Dictionary::create(); \
params->setObject(__String::create(__methodName__), "method");


#endif // __CCSoomlaMacros_h
