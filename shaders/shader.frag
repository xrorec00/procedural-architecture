#version 440 core
out vec4 outColor;
in vec3 normalO;
void main() {
	vec3 light=vec3(-1,1,0);
	float lightIntensity;
	lightIntensity=clamp(dot(normalO,normalize(light)),0,1.0f);
	outColor=vec4(0.1f, 0.1f, 0.1f, 1.0f)+vec4(0.5f, 0.5f, 0.5f, 1.0f)*lightIntensity;
}