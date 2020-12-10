#ifndef EE_X_MESSAGE_HELPER_HPP
#define EE_X_MESSAGE_HELPER_HPP

#include <string>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
/// Used to unify message calls.
class MessageHelper {
public:
    explicit MessageHelper(const std::string& prefix, const std::string& adId);

    /// Generic.
    std::string isLoaded() const;
    std::string load() const;
    std::string onLoaded() const;
    std::string onFailedToLoad() const;
    std::string show() const;
    std::string onFailedToShow() const;
    std::string onClicked() const;
    std::string onClosed() const;

    /// Ad views.
    std::string getPosition() const;
    std::string setPosition() const;
    std::string getSize() const;
    std::string setSize() const;
    std::string isVisible() const;
    std::string setVisible() const;

    /// Interstitial ads.
    std::string createInternalAd() const;
    std::string destroyInternalAd() const;

private:
    std::string prefix_;
    std::string adId_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_MESSAGE_HELPER_HPP
