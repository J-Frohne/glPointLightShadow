#version 400

// Fragment Shader output
layout(location = 0) out vec4 fcolor;

// Light Parameters
uniform vec3 lightPosition; // light position in eye space
uniform vec4 Ia; // ambient light color
uniform vec4 Id; // diffuse light color
uniform vec4 Is; // specular light color

// Shadow map
uniform samplerCubeShadow shadowMap; // shadow map texture
uniform mat4 shadowMapProjection; // light space -> clip space

// Texture sampler
uniform bool hasTexKa;
uniform bool hasTexKd;
uniform bool hasTexKs;
uniform sampler2D texKa; // ambient texture
uniform sampler2D texKd; // diffuse texture
uniform sampler2D texKs; // specular texture

// Material parameters
uniform vec4 ka; // ambient factor
uniform vec4 kd; // diffuse factor
uniform vec4 ks; // specular factor
uniform float shininess; // shininess exponent

// Vertex shader input
smooth in vec4 vpos;           // position in eye space
smooth in vec4 vposLightSpace; // position in light space
smooth in vec2 vtexCoords;     // texture coordinates
smooth in vec3 vnormal;        // normal in eye space, not normalized

// Shadow map computation
// Returns 0 if fragment is in shadow or 1 otherwise
float depthTest(vec3 positionLightSpace)
{
	// Compute the local z value of this fragment in light space
	vec3 absPosLS = abs(positionLightSpace);
	float majorAxisMagnitude = max(absPosLS.x, max(absPosLS.y, absPosLS.z));
	
	// Transform the z value into clip space
	vec4 clipSpace = shadowMapProjection * vec4(0.0, 0.0, -majorAxisMagnitude, 1.0);
	
	// Transform the z value into NDC and map it from [-1,1] to [0,1]
	float depth = (clipSpace.z / clipSpace.w) * 0.5 + 0.5;
	
	// Compare the depth of the fragment to the depth of the shadow map using he sampler
	float shadow = texture(shadowMap, vec4(positionLightSpace, depth));
	
	return shadow;
}

// Phong Lighting
vec4 phong(vec3 N, vec3 L, vec3 V, float shadow)
{
	vec4 ambient  = Ia * (hasTexKa ? texture(texKa, vtexCoords) : ka);
	vec4 diffuse  = Id * (hasTexKd ? texture(texKd, vtexCoords) : kd) * max(dot(N, L), 0.0);
	vec4 specular = Is * (hasTexKs ? texture(texKs, vtexCoords) : ks) 
	                   * pow(max(dot(reflect(-L, N), V), 0.0), shininess)
	                   * ((dot(N, L) > 0.0) ? 1.0 : 0.0);
	
	return ambient + shadow * (diffuse + specular);
}

void main(void)
{
	// Evaluate shadowing
	float shadow = depthTest(vec3(vposLightSpace));
	
	// Compute view, light and normal direction
	vec3 N = normalize(vnormal);
	vec3 V = normalize(vec3(0.0) - vpos.xyz);
	vec3 L = normalize(lightPosition - vpos.xyz);

	// Compute Phong lighting
	vec4 color = phong(N, L, V, shadow);
	
	// Discard transparent fragments
	if(color.a == 0.0)
	{
		discard;
	}
	
	// Write color
	fcolor = color;
}
