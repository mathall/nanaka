#version 100
attribute vec3 position;
attribute vec2 texcoord;
varying vec2 Texcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main() {
	Texcoord = texcoord;
	gl_Position = proj * view * model * vec4(position, 1.0);
}
