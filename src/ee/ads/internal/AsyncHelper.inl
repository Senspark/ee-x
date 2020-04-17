#include <ee/coroutine/LambdaAwaiter.hpp>
#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
#define Self AsyncHelper

template <class T>
bool Self<T>::isProcessing() const {
    return processing_;
}

template <class T>
Task<T> Self<T>::process(const Processor& processor) {
    if (processing_) {
        // Waiting.
    } else {
        awaiter_ = std::make_unique<LambdaAwaiter<T>>(
            [this, processor](auto&& resolve) {
                processing_ = true;
                resolve_ = [this, resolve](T result) {
                    resolve(result);
                    resolve_ = nullptr;
                    awaiter_.reset();
                    processing_ = false;
                };
                processor();
            });
    }
    auto result = co_await(*awaiter_);
    co_return result;
}

template <class T>
void Self<T>::resolve(T result) {
    resolve_(result);
}

#undef Self
} // namespace ads
} // namespace ee
