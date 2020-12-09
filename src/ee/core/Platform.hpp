//
//  PlatformUtils.hpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#ifndef EE_X_PLATFORM_HPP
#define EE_X_PLATFORM_HPP

#ifdef __cplusplus

#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
/// https://developer.android.com/reference/android/view/DisplayCutout
struct SafeInset {
    /// The inset from the left which avoids the display cutout in pixels.
    int left;

    /// The inset from the right which avoids the display cutout in pixels.
    int right;

    /// The inset from the top which avoids the display cutout in pixels.
    int top;

    /// The inset from the bottom which avoids the display cutout in pixels.
    int bottom;
};

class Platform {
public:
    static void registerHandlers(IMessageBridge& bridge);

    /// Checks whether an application with the specified package name (Android)
    /// or scheme (iOS) is installed.
    static bool isApplicationInstalled(const std::string& applicationId);

    /// Opens an application with the specified package name (Android) or scheme
    /// (iOS).
    static bool openApplication(const std::string& applicationId);

    /// Gets the application ID (Android) or bundle identifier (iOS).
    static std::string getApplicationId();

    /// Gets the name of the application.
    static std::string getApplicationName();

    /// Gets the version name of the application.
    static std::string getVersionName();

    /// Gets the version code of the application.
    static std::string getVersionCode();

    /// Android only.
    /// iOS returns an empty string.
    static std::string getSHA1CertificateFingerprint();

    /// Checks whether the current application is an Google Instant application
    /// (Android only).
    static bool isInstantApp();

    /// Checks whether the current device is a table.
    static bool isTablet();

    /// Gets the screen density, i.e. pixel to dp ratio.
    static float getDensity();

    /// Gets the application size in pixels.
    static std::pair<int, int> getViewSize();

    /// Gets the screen size in pixels.
    static std::pair<int, int> getScreenSize();

    /// Gets device's unique ID.
    [[nodiscard]] static Task<std::string> getDeviceId();

    static SafeInset getSafeInset();

    static bool sendMail(const std::string& recipient,
                         const std::string& subject, const std::string& body);

    /// Tests whether the specified host name can be resolved.
    [[nodiscard]] static Task<bool> testConnection(const std::string& hostName,
                                                   float timeOut);

    /// Show Google Instant application installation prompt (Android only).
    static void showInstallPrompt(const std::string& url,
                                  const std::string& referrer);

    [[nodiscard]] static Task<std::string> getInstallReferrerUrl();

private:
    static IMessageBridge* bridge_;
};
} // namespace core

constexpr auto getApplicationId = &Platform::getApplicationId;
constexpr auto getApplicationName = &Platform::getApplicationName;
constexpr auto getDensity = &Platform::getDensity;
constexpr auto getViewSize = &Platform::getViewSize;
constexpr auto getScreenSize = &Platform::getScreenSize;
constexpr auto getDeviceId = &Platform::getDeviceId;
constexpr auto getSafeInset = &Platform::getSafeInset;
constexpr auto getSHA1CertificateFingerprint =
    &Platform::getSHA1CertificateFingerprint;
constexpr auto isInstantApp = &Platform::isInstantApp;
constexpr auto getVersionCode = &Platform::getVersionCode;
constexpr auto getVersionName = &Platform::getVersionName;
constexpr auto isApplicationInstalled = &Platform::isApplicationInstalled;
constexpr auto isTablet = &Platform::isTablet;
constexpr auto openApplication = &Platform::openApplication;
constexpr auto sendMail = &Platform::sendMail;
constexpr auto testConnection = &Platform::testConnection;
constexpr auto getInstallReferrerUrl = &Platform::getInstallReferrerUrl;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLATFORM_UTILS_HPP */
