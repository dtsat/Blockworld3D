#version 330 core

in vec2 textures;
in vec3 out_normal;
in vec3 frag_pos;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

out vec4 frag_colour;	//final output color used to render the point

uniform sampler2D ourTexture;

void main () {
	float ambientStr = 0.4f;
	vec3 ambient = ambientStr * light_color;

	vec3 norm = normalize(out_normal);
	vec3 light_dir = normalize(light_pos - frag_pos);

	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	float specStr = 0.0f;

	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specStr * spec * light_color;

	vec4 color = vec4(ambient + diffuse + specular, 1.0f);
	vec4 texResult = texture(ourTexture, textures);

	frag_colour = color * texResult;
}