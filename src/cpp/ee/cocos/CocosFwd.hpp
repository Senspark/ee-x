#ifndef EE_X_COCOS_FWD_HPP
#define EE_X_COCOS_FWD_HPP

#ifdef __cplusplus

#include <cstddef>
#include <string>
#include <unordered_map>

#include <platform/CCPlatformDefine.h> // For CC_DLL

#include <ee/core/CoreFwd.hpp>

#include "ee/cocos/EEMacro.hpp"

enum class ResolutionPolicy;

namespace cocos2d {
class Node;
class DrawNode;
class Font;
class FontAtlas;
class Label;
class LabelAtlas;
class Layer;
class LayerColor;
class Scene;
class Transition;
class ProgressTimer;
class Menu;
class MenuItem;
class MenuItemImage;
class MenuItemToggle;
class ClippingRectangleNode;
class ClippingNode;
class ParticleSystem;
class ParticleSystemQuad;
class Animation;
class Sprite;
class SpriteFrame;
class SpriteBatchNode;
class Image;
class Texture2D;
class RenderTexture;

struct _ttfConfig;
using TTFConfig = _ttfConfig;

class Action;
class ActionInstant;
class Show;
class Hide;
class RemoveSelf;
class FlipX;
class FlipY;
class Place;
class CallFunc;

class ActionInterval;
class FiniteTimeAction;
class Speed;
class Follow;
class Repeat;
class RepeatForever;
class Spawn;
class RotateTo;
class RotateBy;
class MoveTo;
class MoveBy;
class SkewTo;
class SkewBy;
class ScaleBy;
class ScaleTo;
class Blink;
class FadeTo;
class FadeIn;
class FadeOut;
class DelayTime;
class ActionFloat;
class ActionEase;
class Sequence;

class Event;
class EventCustom;
class EventListener;
class EventListenerCustom;
class EventListenerKeyboard;
class EventListenerTouchOneByOne;
class EventListenerTouchAllAtOnce;
class EventTouch;

class Touch;

class Ref;

template <class T>
class RefPtr;

template <class T>
class Vector;

class Value;
using ValueMap = std::unordered_map<std::string, Value>;

class __Array;
using Array = __Array;

class __Bool;
using Bool = __Bool;

class __Dictionary;
using Dictionary = __Dictionary;

class __Double;
using Double = __Double;

class __Float;
using Float = __Float;

class __Integer;
using Integer = __Integer;

class __Set;
using Set = __Set;

class __String;
using String = __String;

class Vec2;
using Point = Vec2;

class Vec3;
class Mat3;
class Mat4;

class Size;
class Rect;
struct Color3B;
struct Color4F;
struct BlendFunc;

class GLProgram;
class GLProgramState;

struct Uniform;

namespace extension {
class ControlButton;
class ScrollView;
} // namespace extension
} // namespace cocos2d

namespace cocos2d {
namespace ui {
class ListView;
class LoadingBar;
class PageView;
class RichText;
class ScrollView;
class Slider;
class Text;
class TextAtlas;
class Button;
class CheckBox;
class RadioButton;
class ImageView;
class Scale9Sprite;
class Widget;
class EditBox;
class EditBoxDelegate;
class TextField;
class Layout;
} // namespace ui
} // namespace cocos2d

namespace cocos2d {
namespace network {
class HttpClient;
class HttpResponse;
} // namespace network
} // namespace cocos2d

namespace cocosbuilder {
struct BlockData;
struct BlockControlData;

class CCBReader;
class NodeLoaderLibrary;
class NodeLoader;
class SpriteLoader;
class Scale9SpriteLoader;
class ControlLoader;
class LabelBMFontLoader;
class LabelTTFLoader;
class LayerLoader;
class LayerColorLoader;
class LayerGradientLoader;
class ScrollViewLoader;
class ParticleSystemQuadLoader;
class CCBMemberVariableAssigner;
class CCBSelectorResolver;
} // namespace cocosbuilder

namespace spine {
class SkeletonAnimation;
} // namespace spine

namespace ee {
template <class Np, class Lp, class... Ts>
class GenericLoader;

template <class T>
class Pool;
template <class T>
class LazyPtr;

class Action;
class ContinuousAction;

class Sequence;
class Spawn;
class RelativeMoveBy;
class RelativeMoveTo;

namespace detail {
class ButtonEx;
} // namespace detail

using Button = detail::ButtonEx;

class CheckBox;

class BackButtonComponent;

class Console;

template <class T>
class BackButtonListener;

class NodeV3Loader;
class UiWidgetLoader;
class UiButtonLoader;
class UiTextLoader;
class UiLayoutLoader;
class ClippingRectangleNodeLoader;

class ButtonLoader;

class SpriteWithHsv;
class SpriteWithHsvLoader;

class Scale9SpriteWithHsv;

class BlurBackground;
class BlurBackgroundLoader;

namespace language {
class Language;
class ISwitcher;
class Switcher;
class Delegate;
class Formatter;
class MultilingualDelegate;
} // namespace language

using Language = language::Language;
using ILanguageSwitcher = language::ISwitcher;
using LanguageSwitcher = language::Switcher;
using LanguageDelegate = language::Delegate;
using LanguageFormatter = language::Formatter;

namespace ui {
class Widget;
class WidgetLoader;
} // namespace ui

using ui::Widget;
using ui::WidgetLoader;

class ImageBuilder;
class ManagedScene;
class SceneSwitcher;

class Shader;

class SkeletonAnimationLoader;
class SkeletonBone;
class SkeletonBoneLoader;

template <std::size_t Id, class... Args>
class EventInfo;
class EventDispatcher;

template <class T, class = void>
struct DataTraits;

template <class... Keys>
struct DataFormat;

template <std::size_t Id, class Value, class... Args>
struct DataInfo;

class DataHandler;
} // namespace ee

namespace ee {
namespace cocos {
enum class DialogCommandType;

struct DialogCommand;
struct DialogGuard;

class Dialog;
class DialogComponent;
class DialogManager;
class IDialogManager;

class Metrics;

class SwitchToCocosThread;

class CocosBannerAd;
} // namespace cocos

using cocos::Dialog;
using cocos::DialogComponent;
using cocos::Metrics;

using cocos::SwitchToCocosThread;

using cocos::CocosBannerAd;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_COCOS_FWD_HPP
