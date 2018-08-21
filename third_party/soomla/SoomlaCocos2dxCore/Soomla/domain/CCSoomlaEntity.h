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

#ifndef __CCSoomlaEntity_H_
#define __CCSoomlaEntity_H_

#include "CCCoreConsts.h"
#include "CCDomain.h"
#include "CCSoomlaMacros.h"
#include "cocos2d.h"

namespace soomla {
class CCSoomlaEntity : public CCDomain {
public:
    CCSoomlaEntity()
        : mId("")
        , mName("")
        , mDescription("") {}

    virtual bool init(const std::string& id, const std::string& name = "",
                      const std::string& description = "");
    virtual bool initWithValueMap(const cocos2d::ValueMap& dict) override;

    virtual ~CCSoomlaEntity() = default;

    /**
     * Converts this `CCSoomlaEntity` to a `CCDictionary`.
     * @return `CCDictionary` representation of this `CCSoomlaEntity`.
     */
    virtual cocos2d::ValueMap toValueMap();
    
    virtual char const* getType() const = 0;

    virtual bool equals(cocos2d::Ref* obj) const;

    virtual CCSoomlaEntity* clone(const std::string& newId);

protected:
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mId, Id);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mName, Name);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mDescription, Description);

    void fillIdFromValueMap(const cocos2d::ValueMap& dict);
    void putIdToValueMap(cocos2d::ValueMap& dict);
    void fillNameFromValueMap(const cocos2d::ValueMap& dict);
    void putNameToValueMap(cocos2d::ValueMap& dict);
    void fillDescriptionFromValueMap(const cocos2d::ValueMap& dict);
    void putDescriptionToValueMap(cocos2d::ValueMap& dict);
};
} // namespace soomla

#endif //__CCSoomlaEntity_H_
