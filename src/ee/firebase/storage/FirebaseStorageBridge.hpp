//
//  FirebaseStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_STORAGE_BRIDGE_HPP
#define EE_X_FIREBASE_STORAGE_BRIDGE_HPP

#include <array>
#include <functional>
#include <memory>
#include <string>

#include <ee/FirebaseFwd.hpp>

namespace firebase {
namespace storage {
class Storage;
class Metadata;
} // namespace storage
} // namespace firebase

namespace ee {
namespace firebase {
namespace storage {
/// Wrapper for Firebase Storage.
/// Should have a single instance at a time.
/// https://firebase.google.com/docs/storage/cpp/start
/// https://firebase.google.com/docs/reference/cpp/namespace/firebase/storage
class Bridge final {
public:
    using HashCallback =
        std::function<void(bool succeeded, const std::string& hash)>;
    using DataCallback =
        std::function<void(bool succeeded, const std::string& data)>;

    Bridge();
    ~Bridge();

    /// Attempts to initialize the remote config.
    /// @return True if successfully initialized, false otherwise.
    bool initialize();

    /// @return The maximum time in seconds to retry a download if a failure
    /// occurs, -1 if not successfully initialized or not supported.
    double getMaxDownloadRetryTime() const;

    /// @return The maximum time to retry an upload if a failure occurs, -1 if
    /// not successfully initialized or not supported.
    double getMaxUploadRetryTime() const;

    /// @return The maximum time to retry operations other than upload and
    /// download if a failure occurs, -1 if not successfully initialized or not
    /// supported.
    double getMaxOperationRetryTime() const;

    /// Sets the maximum time to retry a download if a failure occurs.
    /// Defaults to 600 seconds (10 minutes).
    void setMaxDownloadRetryTime(double seconds);

    /// Sets the maximum time to retry operations other than upload and download
    /// if a failure occurs.
    /// Defaults to 120 seconds (2 minutes).
    void setMaxOperationRetryTime(double seconds);

    /// Sets the maximum time to retry an upload if a failure occurs.
    /// Defaults to 600 seconds (10 minutes).
    void setMaxUploadRetryTime(double seconds);

    void getHash(const std::string& filePath, const HashCallback& callback);
    void getData(const std::string& filePath, const DataCallback& callback);

private:
    static constexpr std::size_t max_file_size_in_bytes = 20000;

    bool initialized_;
    bool fetching_;

    ::firebase::storage::Storage* storage_;

    std::unique_ptr<Scheduler<::firebase::storage::Metadata>>
        metadataScheduler_;
    std::unique_ptr<Scheduler<std::size_t>> bytesScheduler_;

    std::array<char, max_file_size_in_bytes> buffer_;
};
} // namespace storage
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_STORAGE_BRIDGE_HPP */
