//
//  EEForward.hpp
//  ee-library.
//
//  Created by Hoang Le Hai on 7/20/15.
//
//

#ifndef EE_LIBRARY_FORWARD_HPP_
#define EE_LIBRARY_FORWARD_HPP_

#include <cstddef>

/// Forward ee library classes.

namespace ee {
template <class Np, class Lp, class... Ts>
class GenericLoader;

template <class T>
class DynamicValue;
template <class T>
class Pool;
template <class T>
class LazyPtr;

class ScopeGuard;

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

namespace dialog {
class Dialog;
class DialogComponent;
class DialogManager;
} // namespace dialog

using dialog::Dialog;
using dialog::DialogComponent;
using dialog::DialogManager;

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

#endif // EE_LIBRARY_FORWARD_HPP_
