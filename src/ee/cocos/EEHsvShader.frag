//
//  EEHSVShader.frag
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

constexpr auto ee_hsv_shader_frag = R"(
#ifdef GL_ES
precision mediump float;
#endif // GL_ES

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

/// HSV matrix.
uniform mat4 u_hsv;

void main() {
    vec4 pixelColor = texture2D(CC_Texture0, v_texCoord);
    
    // Store the original alpha.
    float alpha = pixelColor.w;
    
    // Reset alpha to 1.0.
    pixelColor.w = 1.0;
    
    vec4 fragColor = u_hsv * pixelColor;
    
    // Restore the original alpha.
    fragColor.w = alpha;
    gl_FragColor = fragColor * v_fragmentColor;
})";
