#version 400

// Fragment Shader output
layout(location = 0) out vec4 fcolor;

// Light Parameters
uniform vec3 lightPosition; // light position in eye space
uniform vec4 Ia; // ambient light color
uniform vec4 Id; // diffuse light color
uniform vec4 Is; // specular light color

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
smooth in vec2 vtexCoords;     // texture coordinates
smooth in vec3 vnormal;        // normal in eye space, not normalized

// Phong Lighting
vec4 phong(vec3 N, vec3 L, vec3 V)
{
	vec4 ambient  = Ia * (hasTexKa ? texture(texKa, vtexCoords) : ka);
	vec4 diffuse  = Id * (hasTexKd ? texture(texKd, vtexCoords) : kd) * max(dot(N, L), 0.0);
	vec4 specular = Is * (hasTexKs ? texture(texKs, vtexCoords) : ks) 
	                   * pow(max(dot(reflect(-L, N), V), 0.0), shininess)
	                   * ((dot(N, L) > 0.0) ? 1.0 : 0.0);
	
	return ambient + diffuse + specular;
}

void main(void)
{
	// Compute view, light and normal direction
	vec3 N = normalize(vnormal);
	vec3 V = normalize(vec3(0.0) - vpos.xyz);
	vec3 L = normalize(lightPosition - vpos.xyz);

	// Compute Phong lighting
	vec4 color = phong(N, L, V);
	
	// Discard transparent fragments
	if(color.a == 0.0)
	{
		discard;
	}
	
	// Write color
	fcolor = color;
}
