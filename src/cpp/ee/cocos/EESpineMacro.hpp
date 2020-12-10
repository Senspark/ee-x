//
//  EESpineMacro.hpp
//  ee-library
//
//  Created by Zinge on 5/4/17.
//
//

#ifndef EE_LIBRARY_SPINE_MACRO_HPP_
#define EE_LIBRARY_SPINE_MACRO_HPP_

// clang-format off
/// There is no way to detect the spine runtime version.
/// Spine runtime for v3 has a new header called Cocos2dAttachmentLoader.h.
#if __has_include(<spine/Cocos2dAttachmentLoader.h>)
#define EE_SPINE_RUNTIME_V3
#else // __has_include(<spine/Cocos2dAttachmentLoader.h>)
#define EE_SPINE_RUNTIME_V2
#endif // __has_include(<spine/Cocos2dAttachmentLoader.h>)
// clang-format on

#endif /* EE_LIBRARY_SPINE_MACRO_HPP_ */
