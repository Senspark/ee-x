//
//  EECocos2dxFwd.hpp
//  ee-library
//
//  Created by enrevol on 3/22/16.
//
//

#ifndef EE_LIBRARY_COCOS2DX_FWD_HPP_
#define EE_LIBRARY_COCOS2DX_FWD_HPP_

/// Forward cocos2d-x classes.

#include <string>
#include <unordered_map>

#include <platform/CCPlatformDefine.h> // For CC_DLL

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

#endif /* EE_LIBRARY_COCOS2DX_FWD_HPP_ */
