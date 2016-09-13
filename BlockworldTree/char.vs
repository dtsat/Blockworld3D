#version 330 core

uniform mat4 view_matrix, model_matrix, proj_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 textures;
out vec3 out_normal;
out vec3 frag_pos;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM * vec4 (position, 1.0);
	textures = vec2(texCoord.x, 1.0 - texCoord.y);
	frag_pos = vec3(model_matrix * vec4(position, 1.0));
	out_normal = normal;
}