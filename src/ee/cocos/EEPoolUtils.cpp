//
//  EEParticlePool.cpp
//  ee-library
//
//  Created by Zinge on 8/1/17.
//
//

#include "EEPoolUtils.hpp"
#include "EEPool.hpp"
#include "EEAction.hpp"

#include <2d/CCParticleSystemQuad.h>
#include <2d/CCSpriteFrameCache.h>

namespace ee {
namespace detail {
class ParticleWithCleanup : public cocos2d::ParticleSystemQuad {
private:
    using Self = ParticleWithCleanup;
    using Super = cocos2d::ParticleSystemQuad;

public:
    static Self* create(const std::string& filename);

    virtual void removeFromParentAndCleanup(bool cleanup) override;
};

ParticleWithCleanup* ParticleWithCleanup::create(const std::string& filename) {
    auto result = new (std::nothrow) Self();
    if (result != nullptr && result->initWithFile(filename)) {
        result->autorelease();
    } else {
        CC_SAFE_DELETE(result);
    }
    return result;
}

void ParticleWithCleanup::removeFromParentAndCleanup(bool cleanup) {
    // Hacky way to reset the texture.
    ParticleSystem::setTexture(nullptr);
    Super::removeFromParentAndCleanup(cleanup);
}
} // namespace detail

namespace {
using ParticlePool = ee::Pool<cocos2d::ParticleSystemQuad>;

/// Gets the particle pools.
std::map<std::string, ParticlePool>& getPools() {
    static std::map<std::string, ParticlePool> pools;
    return pools;
}

ParticlePool& getPool(const std::string& plistName) {
    auto&& pools = getPools();
    auto iter = pools.find(plistName);
    if (iter == pools.cend()) {
        auto constructor = [plistName] {
            auto emitter = detail::ParticleWithCleanup::create(plistName);
            emitter->setAutoRemoveOnFinish(true);
            return emitter;
        };
        auto destructor = [](cocos2d::ParticleSystemQuad* instance) {
            instance->resetSystem();
        };
        iter =
            pools.emplace(std::piecewise_construct,
                          std::forward_as_tuple(plistName),
                          std::forward_as_tuple(constructor, destructor)).first;
    }
    return iter->second;
}
} // namespace

void initializeParticlePool(const std::string& plistName,
                            std::size_t capacity) {
    auto&& pool = getPool(plistName);
    pool.reserve(capacity);
}

cocos2d::ParticleSystem* createParticle(const std::string& plistName,
                                        const std::string& textureName) {
    auto&& pool = getPool(plistName);

    auto frameCache = cocos2d::SpriteFrameCache::getInstance();
    auto frame = frameCache->getSpriteFrameByName(textureName);

    auto particle = pool.pop();
    // Change the particle texture.
    particle->setTextureWithRect(frame->getTexture(), frame->getRect());
    return particle;
}
} // namespacve ee
