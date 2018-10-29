//
//  EEUtils.cpp
//  ee-library
//
//  Created by Zinge on 4/23/16.
//
//

#include "EEUtils.hpp"
#include "EEImage.hpp"

#include <2d/CCNode.h>
#include <2d/CCRenderTexture.h>
#include <2d/CCTransition.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListener.h>
#include <base/CCEventListenerCustom.h>
#include <base/CCRefPtr.h>
#include <base/CCScheduler.h>
#include <network/HttpClient.h>
#include <network/HttpRequest.h>
#include <network/HttpResponse.h>
#include <platform/CCImage.h>
#include <renderer/CCCustomCommand.h>
#include <renderer/CCRenderer.h>
#include <renderer/CCTextureCache.h>

NS_EE_BEGIN
UniqueListener make_unique_listener(cocos2d::EventListener* listener) {
    auto deleter = [](cocos2d::EventListener* lst) {
        cocos2d::Director::getInstance()
            ->getEventDispatcher()
            ->removeEventListener(lst);
    };
    return UniqueListener(listener, deleter);
}

void doRecursively(cocos2d::Node* node,
                   const std::function<void(cocos2d::Node*)>& action) {
    auto&& children = node->getChildren();
    for (auto&& child : children) {
        doRecursively(child, action);
    }
    action(node);
}

void pauseAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::pause);
}

void resumeAll(cocos2d::Node* node) {
    doRecursively(node, &cocos2d::Node::resume);
}

cocos2d::Rect getCascadeContentSize(const cocos2d::Node* node,
                                    std::size_t depth) {
    auto&& size = node->getContentSize();
    cocos2d::Rect result{0, 0, size.width, size.height};
    if (depth > 0) {
        for (auto&& child : node->getChildren()) {
            auto rect = getCascadeContentSize(child, depth - 1);
            rect = cocos2d::RectApplyAffineTransform(
                rect, child->getNodeToParentAffineTransform());
            result.merge(rect);
        }
    }
    return result;
}

bool isActuallyVisible(const cocos2d::Node* node) {
    if (node == nullptr) {
        // node is nullptr.
        return false;
    }
    if (node->isRunning() == false) {
        // node is not running.
        return false;
    }
    auto current = node;
    while (current != nullptr) {
        if (current->isVisible() == false) {
            // One of node's ancestors is not visible.
            return false;
        }
        current = current->getParent();
    }
    return true;
}

namespace {
void onCaptured(const std::function<void(cocos2d::Image*)>& afterCaptured) {
    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    auto frameSize = glView->getFrameSize();

    GLsizei width = static_cast<GLsizei>(frameSize.width);
    GLsizei height = static_cast<GLsizei>(frameSize.height);

    do {
        auto bufferSize = static_cast<std::size_t>(width * height * 4);
        auto buffer = std::make_unique<GLubyte[]>(bufferSize);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                     buffer.get());

        auto flippedBuffer = std::make_unique<GLubyte[]>(bufferSize);
        for (int row = 0; row < height; ++row) {
            std::memcpy(flippedBuffer.get() + (height - row - 1) * width * 4,
                        buffer.get() + row * width * 4,
                        static_cast<std::size_t>(width) * 4);
        }

        auto image = new cocos2d::Image();
        image->initWithRawData(flippedBuffer.get(), width * height * 4, width,
                               height, 8);
        auto guard = make_ref_guard(image);
        image->release();
        director->getScheduler()->performFunctionInCocosThread(
            [image, guard, afterCaptured] { afterCaptured(image); });
    } while (false);
}
} // namespace

void captureScreenInPixels(
    const std::function<void(cocos2d::Image*)>& afterCaptured) {
    static cocos2d::EventListenerCustom* listener;
    if (listener != nullptr) {
        CCLOG("Warning: CaptureScreen has been called already, don't call more "
              "than once in one frame.");
        return;
    }

    listener = cocos2d::Director::getInstance()
                   ->getEventDispatcher()
                   ->addCustomEventListener(
                       cocos2d::Director::EVENT_AFTER_DRAW,
                       [afterCaptured](cocos2d::EventCustom* event) {
                           auto director = cocos2d::Director::getInstance();
                           director->getEventDispatcher()->removeEventListener(
                               listener);
                           listener = nullptr;
                           auto renderer = director->getRenderer();

                           cocos2d::CustomCommand command;
                           command.init(std::numeric_limits<float>::max());
                           command.func = std::bind(&onCaptured, afterCaptured);

                           renderer->addCommand(&command);
                           renderer->render();
                       });
}

cocos2d::Image* captureScreenInPoints(float scale) {
    auto director = cocos2d::Director::getInstance();

    // Retrieve the current scene.
    auto scene = director->getRunningScene();

    auto transition = dynamic_cast<cocos2d::TransitionScene*>(scene);
    if (transition != nullptr) {
        // Current scene is a transition scene,
        // which means this function is called when the transition
        // scene has not finished.
        CCLOG("You are capturing the transition scene!");
    }

    // Retrieve the win size.
    auto&& winSize = director->getWinSize();

    // Calculate resulting image size.
    auto&& size = winSize * scale;

    int width = static_cast<int>(size.width);
    int height = static_cast<int>(size.height);

    // Create a renderer.
    auto renderer = cocos2d::RenderTexture::create(
        width, height, cocos2d::Texture2D::PixelFormat::RGBA8888,
        GL_DEPTH24_STENCIL8);

    renderer->setKeepMatrix(true);

    // Process rendering.
    renderer->begin();

    scene->visit();

    renderer->end();

    // Force to render immediately.
    director->getRenderer()->render();

    // Retrieve the image.
    auto image = renderer->newImage();
    image->autorelease();

    return image;
}

cocos2d::Sprite* captureBlurredScreenInPoints(float scale, int blurRadius,
                                              int iterations) {
    auto image = captureScreenInPoints(scale);
    auto realRadius =
        static_cast<image::SizeType>(blurRadius * CC_CONTENT_SCALE_FACTOR());
    return createSpriteFromImage(
        image,
        std::bind(tentBlur1D, std::placeholders::_1, realRadius, iterations));
}

cocos2d::Sprite*
createSpriteFromImage(cocos2d::Image* image,
                      const std::function<void(cocos2d::Image*)>& processor) {

    if (processor) {
        processor(image);
    }

    auto texture = new cocos2d::Texture2D();
    texture->initWithImage(image);

    // Create the resulting sprite.
    auto sprite = cocos2d::Sprite::createWithTexture(texture);

    // Release resource.
    texture->release();

    return sprite;
}

void downloadImage(
    const std::string& imageUrl,
    const std::function<void(cocos2d::Texture2D*)>& afterDownloaded) {
    // Retrieve the texture in the texture cache.
    auto cachedTexture =
        cocos2d::Director::getInstance()->getTextureCache()->getTextureForKey(
            imageUrl);

    if (cachedTexture != nullptr) {
        // Image is already in the cache.
        return afterDownloaded(cachedTexture);
    }

    // Image is not in the cache.
    // Attempt to download the image from the given url.

    // Can not use std::unique_ptr here because cocos2d::network::HttpClient
    // will retain it later.
    auto request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(imageUrl.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);

    auto callback = [imageUrl, afterDownloaded](
        cocos2d::network::HttpClient* client,
        cocos2d::network::HttpResponse* response) {
        cocos2d::Image* image = nullptr;
        do {
            CC_BREAK_IF(response == nullptr || response->isSucceed() == false);
            CC_BREAK_IF(response->getResponseCode() != 200);

            auto buffer = response->getResponseData();
            CC_BREAK_IF(buffer == nullptr);
            CC_BREAK_IF(buffer->size() == 0);

            // GIF images are not supported by cocos2d-x.
            CC_BREAK_IF(buffer->size() < 3);
            CC_BREAK_IF((*buffer)[0] == 'G' && (*buffer)[1] == 'I' &&
                        (*buffer)[2] == 'F');

            auto data = reinterpret_cast<unsigned char*>(&buffer->front());
            auto dataLen = static_cast<ssize_t>(buffer->size());

            image = new (std::nothrow) cocos2d::Image();
            image->initWithImageData(data, dataLen);
        } while (false);

        if (afterDownloaded) {
            // Current thread is not cocos thread,
            // we should invoke the callback on cocos thread.
            cocos2d::Director::getInstance()
                ->getScheduler()
                ->performFunctionInCocosThread(
                    [afterDownloaded, imageUrl, image] {
                        if (image != nullptr) {
                            // Create new texture with the given image and url.
                            auto newTexture = cocos2d::Director::getInstance()
                                                  ->getTextureCache()
                                                  ->addImage(image, imageUrl);

                            // Invoke the callback.
                            afterDownloaded(newTexture);

                            // Release resources.
                            image->release();
                        } else {
                            afterDownloaded(nullptr);
                        }
                    });
        }
    };
    request->setResponseCallback(callback);
    cocos2d::network::HttpClient::getInstance()->sendImmediate(request);

    // Release resources.
    request->release();
}
NS_EE_END
