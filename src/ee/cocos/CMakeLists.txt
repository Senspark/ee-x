cmake_minimum_required(VERSION 3.1)

project(ee_x_cocos)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wchar-subscripts -Wcomment -Wformat=2 -Wnonnull -Winit-self")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wreturn-type -Waddress -Wmissing-declarations -Wmissing-field-initializers")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-function -Wunused-label -Wuninitialized")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
    ./
)

set(EE_X_COCOS_SRC
    EESceneSwitcher.cpp
    EEDialogComponent.cpp
    EESha1.cpp
    EEDialogGuard.cpp
    EEUtils.cpp
    EEClippingRectangleNodeLoader.cpp
    EEImageBuilder.cpp
    EEDataUtils.cpp
    EEImage.cpp
    EEBlurBackground.cpp
    EEButtonLoader.cpp
    EESpriteWithHsvLoader.cpp
    EEUiTextLoader.cpp
    EELanguageDelegate.cpp
    EEHsvProtocol.cpp
    EEContinuousAction.cpp
    EESkeletonAnimationLoader.cpp
    EEShaderUtils.cpp
    EEAction.cpp
    EEScale9SpriteWithHsv.cpp
    EELanguageFormatter.cpp
    EEUiWidgetCallback.cpp
    EESkeletonBone.cpp
    EEBlurBackgroundLoader.cpp
    EEDialogCommand.cpp
    EEBoxBlur1D.cpp
    EEDialogManager.cpp
    EEDialogTree.cpp
    EEDynamicValue.cpp
    EEClippingRectangleNode.cpp
    EEBackButtonComponent.cpp
    EEUiWidgetLoader.cpp
    EEConsole.cpp
    EEUiCheckBoxLoader.cpp
    EEDataHandler.cpp
    EETransitionFadeOut.cpp
    EEButton.cpp
    EEMd5.cpp
    EEBackButtonListener.cpp
    EEPreferences.cpp
    EEUiLayoutLoader.cpp
    EESpineFactory.cpp
    EEShaderManager.cpp
    EELanguage.cpp
    EECrc.cpp
    EEBoxBlur2D.cpp
    EEPoolUtils.cpp
    EENodeV3Loader.cpp
    EEDialogUtils.cpp
    EELanguageSwitcher.cpp
    EEEventDispatcher.cpp
    EEMultilingualDelegate.cpp
    EEUiButtonLoader.cpp
    EEConsoleDelegate.cpp
    EEDataTraits.cpp
    EETentBlur1D.cpp
    EEShader.cpp
    EESpriteWithHsv.cpp
    EEManagedScene.cpp
    EESkeletonBoneLoader.cpp
    EEDialog.cpp
    EECheckBox.cpp
    EECommandPool.cpp
)

set(EE_X_COCOS_HEADERS
    EEDataInfo.hpp
    EEMeta.hpp
    EEPool.hpp
    EEEvent.hpp
    EEDataTraits.hpp
    EEShader.hpp
    EESpriteWithHsv.hpp
    EEManagedScene.hpp
    EESkeletonBoneLoader.hpp
    EEHeader.hpp
    EEDialog.hpp
    EECheckBox.hpp
    EECommandPool.hpp
    EEButton.hpp
    EECocos2dxFwd.hpp
    EEEventListener.hpp
    EEBackButtonListener.hpp
    EEUiLayoutLoader.hpp
    EEPreferences.hpp
    EESpineFactory.hpp
    EEShaderManager.hpp
    EELanguage.hpp
    EEPoolUtils.hpp
    EEDialogUtils.hpp
    EENodeV3Loader.hpp
    EELanguageSwitcher.hpp
    EEDataMeta.hpp
    EEEventDispatcher.hpp
    EEMultilingualDelegate.hpp
    EEUiButtonLoader.hpp
    EEConsoleDelegate.hpp
    EELanguageFormatter.hpp
    EEAction.hpp
    EEScale9SpriteWithHsv.hpp
    EEBlurBackgroundLoader.hpp
    EEDialogCommand.hpp
    EESkeletonBone.hpp
    EEUiWidgetCallback.hpp
    EEDialogTree.hpp
    EEDialogManager.hpp
    EEDynamicValue.hpp
    EELazyPtr.hpp
    EEBackButtonComponent.hpp
    EEClippingRectangleNode.hpp
    EEGenericLoader.hpp
    EEUiCheckBoxLoader.hpp
    EEUiWidgetLoader.hpp
    EEConsole.hpp
    EEForward.hpp
    EEScopeGuard.hpp
    EETransitionFadeOut.hpp
    EEDataHandler.hpp
    EEFlags.hpp
    EESceneSwitcher.hpp
    EEDialogComponent.hpp
    EEDialogGuard.hpp
    EEUtils.hpp
    EEClippingRectangleNodeLoader.hpp
    EEImageBuilder.hpp
    EEDataUtils.hpp
    EEMacro.hpp
    EEImage.hpp
    EEButtonLoader.hpp
    EEBlurBackground.hpp
    EESpriteWithHsvLoader.hpp
    EEUiTextLoader.hpp
    EECrytography.hpp
    EEHsvProtocol.hpp
    EELanguageDelegate.hpp
    EESpineMacro.hpp
    EESkeletonAnimationLoader.hpp
    EEContinuousAction.hpp
    EEShaderUtils.hpp
)

add_library(${PROJECT_NAME} STATIC ${EE_X_COCOS_SRC} ${EE_X_COCOS_HEADERS})

target_link_libraries(${PROJECT_NAME}
    cocos2d
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ./
)