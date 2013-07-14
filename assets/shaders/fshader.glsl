#version 100
precision mediump float;
uniform vec4 color;
varying vec2 Texcoord;
varying vec4 outColor;
uniform sampler2D tex;
void main() {
	gl_FragColor = color * texture2D(tex, Texcoord);
}
