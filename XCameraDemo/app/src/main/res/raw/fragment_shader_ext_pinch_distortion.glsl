#extension GL_OES_EGL_image_external : require
precision mediump float; //指定默认精度

// MUST highp, otherwise will bad result on some device
varying highp vec2 vTextureCoord;
uniform samplerExternalOES uTexture;

uniform float uAspectRatio;
uniform vec2 uCenter;
uniform float uRadius;
uniform float uScale;

void main() {
    float x = vTextureCoord.x;
    float y = (vTextureCoord.y - uCenter.y) * uAspectRatio + uCenter.y;
    vec2 textureCoordinateToUse = vec2(x, y);

    float dist = distance(uCenter, textureCoordinateToUse);
    textureCoordinateToUse = vTextureCoord;

    if (dist < uRadius) {
        textureCoordinateToUse -= uCenter;
        float percent = 1.0 + ((uRadius - dist) / uRadius) * uScale;

        textureCoordinateToUse = textureCoordinateToUse * percent;
        textureCoordinateToUse += uCenter;
    }

    gl_FragColor = texture2D(uTexture, textureCoordinateToUse);
}