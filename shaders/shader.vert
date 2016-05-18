#version 440 core
in vec3 position;
in vec3 normal;
out vec3 normalO;
uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;
void main() {
	normalO=vec4(normal,0).xyz;
	gl_Position = proj*view*model*vec4(position, 1.0);
}