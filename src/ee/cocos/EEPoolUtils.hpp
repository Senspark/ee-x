//
//  EEParticlePool.hpp
//  ee-library
//
//  Created by Zinge on 8/1/17.
//
//

#ifndef EE_LIBRARY_PARTICLE_POOL_HPP
#define EE_LIBRARY_PARTICLE_POOL_HPP

#include "EECocos2dxFwd.hpp"

namespace ee {
/// Pre-allocates particles.
/// @param[in] plistName The name of the plist file.
/// @param[in] capacity The number of pre-allocate particles.
void initializeParticlePool(const std::string& plistName, std::size_t capacity);

/// Creates and caches a particle with the specified plist name and texture
/// name.
/// @param[in] plistName The name of the plist.
/// @param[in] textureName The name of the texture.
/// @return A particle.
cocos2d::ParticleSystem* createParticle(const std::string& plistName,
                                        const std::string& textureName);
} // namespace ee

#endif /* EE_LIBRARY_PARTICLE_POOL_HPP */
