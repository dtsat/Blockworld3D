#version 330 core

in vec2 textures;
in vec3 out_normal;
in vec3 frag_pos;
in vec4 FragPosLightSpace;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

out vec4 frag_colour;	//final output color used to render the point

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(out_normal);
    vec3 lightDir = normalize(light_pos - frag_pos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


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
	float shadow = ShadowCalculation(FragPosLightSpace);                      
    shadow = min(shadow, 0.75);
	vec3 color = (ambient + (1.0 - shadow) * (diffuse + specular));
	vec4 texResult = texture(ourTexture, textures);
	frag_colour = vec4(color, 1.0f) * texResult;

}