//
//  EEHSVShader.frag
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

constexpr auto ee_hsv_shader_vert = R"(
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else // GL_ES
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif // GL_ES

void main() {
    gl_Position = CC_PMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
})";
