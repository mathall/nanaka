#ifdef GL_ES
precision mediump float;
#endif // GL_ES
uniform vec4 color;
varying vec2 Texcoord;
varying vec4 outColor;
void main() {
	gl_FragColor = color;
}
