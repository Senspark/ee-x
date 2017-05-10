//
//  FirebaseStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_STORAGE_HPP_
#define EE_X_FIREBASE_STORAGE_HPP_

#include <array>
#include <functional>
#include <memory>
#include <string>

namespace firebase {
namespace storage {
class Storage;
class Metadata;
} // namespace storage
} // namespace

namespace ee {
namespace firebase {
template <class T> class FirebaseScheduler;

class FirebaseStorage final {
public:
    using HashCallback =
        std::function<void(bool succeeded, const std::string& hash)>;
    using DataCallback =
        std::function<void(bool succeeded, const std::string& data)>;

    FirebaseStorage();
    ~FirebaseStorage();

    bool initialize();

    void getHash(const std::string& filePath, const HashCallback& callback);
    void getData(const std::string& filePath, const DataCallback& callback);

private:
    static constexpr std::size_t max_file_size_in_bytes = 20000;

    bool initialized_;
    bool fetching_;

    ::firebase::storage::Storage* storage_;

    std::unique_ptr<FirebaseScheduler<::firebase::storage::Metadata>>
        metadataScheduler_;
    std::unique_ptr<FirebaseScheduler<std::size_t>> bytesScheduler_;

    std::array<char, max_file_size_in_bytes> buffer_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_STORAGE_HPP_ */
