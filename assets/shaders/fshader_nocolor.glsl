#ifdef GL_ES
precision mediump float;
#endif // GL_ES
varying vec2 Texcoord;
varying vec4 outColor;
uniform sampler2D tex;
void main() {
	gl_FragColor = texture2D(tex, Texcoord);
}
