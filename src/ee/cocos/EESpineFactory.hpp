//
//  EESpineFactory.hpp
//  ee-library
//
//  Created by Zinge on 1/9/17.
//
//

#ifndef EE_LIBRARY_SPINE_FACTORY_HPP_
#define EE_LIBRARY_SPINE_FACTORY_HPP_

#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "ee/CocosFwd.hpp"

struct spAtlas;
struct spSkeletonData;

namespace ee {
namespace detail {
struct SpineData;

using SpineAtlasDeleter = std::function<void(spAtlas* atlas)>;
using SpineAtlasPtr = std::unique_ptr<spAtlas, SpineAtlasDeleter>;
using SpineDataDeleter = std::function<void(detail::SpineData* data)>;
using SpineDataPtr = std::unique_ptr<SpineData, SpineDataDeleter>;
} // namespace detail

class SpineFactory final {
public:
    static SpineFactory* getInstance();

    /// Creates a spine animation with the specified data filename (json), atlas
    /// filename and scale.
    /// @param dataFile The data filename.
    /// @param atlasFile The atlas filename.
    /// @param scale The scale of the skeleton.
    /// @return An auto-release skeleton animation.
    /// @note The data filename should be matched with the atlas filename or
    /// there will be an assertion.
    spine::SkeletonAnimation* createAnimation(const std::string& dataFile,
                                              const std::string& atlasFile,
                                              float scale);

    /// Gets (or creates if not cached) skeleton data for the specified data
    /// filename, atlas filename and scale.
    /// @return Pointer to the cached skeleton data.
    /// @warning Caller must not delete the pointer.
    spSkeletonData* getSkeletonData(const std::string& dataName,
                                    const std::string& atlasName, float scale);

    /// Gets (or creates if not cached) atlas for the specified atlas filename.
    /// @return Pointer to the cached atlas.
    /// @warning Caller must not delete the pointer.
    spAtlas* getAtlas(const std::string& atlasName);

    void destroy(const std::string& dataName, const std::string& atlasName);

private:
    SpineFactory() = default;
    ~SpineFactory() = default;

    /// Stores (atlas filename, skeleton json).
    std::unordered_map<std::string, detail::SpineAtlasPtr> cachedAtlas_;

    /// Stores (data filename, skeleton data).
    std::map<std::pair<std::string, float>, detail::SpineDataPtr>
        cachedSkeletonData_;
};
} // namespace ee

#endif /* EE_LIBRARY_SPINE_FACTORY_HPP_ */
