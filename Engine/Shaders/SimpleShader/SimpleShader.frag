#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in mat3 fTBN;
in vec3 fVertPos;
in vec3 fViewPos;

struct Material {
	sampler2D baseColourMap;
	sampler2D specularMap;
	sampler2D normalMap;
	float shininess;
	float specularStrength;
	float brightness;
};

// Material for the shader to interface with our engine material
uniform Material material;

struct DirLight {
	vec3 colour;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

struct PointLight {
	vec3 colour;
	vec3 position;
	float intensity;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 colour;
	vec3 position;
	vec3 direction;

	float coneAngle;
	float innerCutOff;
	float outerCutOff;

	float intensity;
	float linear;
	float quadratic;
};

#define NUM_DIR_LIGHTS 2
uniform DirLight dirLights[NUM_DIR_LIGHTS];
uniform int addedDirLights = 0;

#define NUM_POINT_LIGHTS 20
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform int addedPointLights = 0;

#define NUM_SPOT_LIGHTS 20
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform int addedSpotLights = 0;

out vec4 finalColour;

uniform float brightness = 1.0f;

void main() {
	// Final colour result for the vertex
	vec3 result = vec3(0.0f);

	// Remove transparent pixels
	// Found discard function from:
	// Victor Gordon 2021, OpenGL Tutorial 17 - Transparency & Blending, viewed August 8, https://www.youtube.com/watch?v=crOfyWiWxmc
	if (texture(material.baseColourMap, fTexCoords).a < 0.1f) discard;

	// Base colour map value that the object starts as
	vec3 baseColour = texture(material.baseColourMap, fTexCoords).rgb * fColour * material.brightness;

	// Specular map value that the object starts as
	vec3 specularColour = texture(material.specularMap, fTexCoords).rgb;

	// Normal colour map value that the object starts as
	vec3 normalColour = texture(material.normalMap, fTexCoords).rgb;
	vec3 normals = normalize(normalColour * 2.0f - 1.0f);

	// Normalise the texture normals based on the TBN matrix
	normals = normalize(fTBN * normals);

	// Get the view direction
	vec3 viewDir = normalize(fViewPos - fVertPos);

	// ------------ DIRECTIONAL LIGHTS
	for (int i = 0; i < addedDirLights; ++i) {
		// Material light direction
		vec3 lightDir = normalize(-dirLights[i].direction);

		// Get the reflection light value
		vec3 reflectDir = reflect(-lightDir, normals);

		// How much light should show colour based on direction of normal facing the light
		float diff = max(dot(normals, lightDir), 0.0f);

		// Ambient algorithm
		// Minimum light value
		vec3 ambientLight = baseColour * dirLights[i].ambient;

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * dirLights[i].colour;
		lightColour *= diff;
		lightColour *= dirLights[i].intensity;

		// Specular power algorithm
		// Caulculate the shininess of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= dirLights[i].intensity;

		// Add our light values together to get the results
		result += ambientLight + lightColour + specular;
	}

	// ------------ POINT LIGHTS
	for (int i = 0; i < addedPointLights; ++i) {
		// Light direction from the point light to the vertex
		vec3 lightDir = normalize(pointLights[i].position - fVertPos);

		// Get the reflection light value
		vec3 reflectDir = reflect(-lightDir, normals);

		// How much light should show colour based on direction of normal facing the light
		float diff = max(dot(normals, lightDir), 0.0f);

		// Distance between the lights position and vertex position
		float distance = length(pointLights[i].position - fVertPos);

		// Actual attenuation calculation
		float attenCalc = 1.0f 
			+ pointLights[i].linear * distance 
			+ pointLights[i].quadratic * (distance * distance);
		
		// Distance that the light can reach
		// Value between 1 and 0, 1 is full light and 0 is no light
		float attenuation = 0.0f;

		// Ensure no division by 0
		if (attenCalc != 0.0f) {
			attenuation = 1.0f / attenCalc;
		}

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * pointLights[i].colour;
		lightColour *= diff;
		lightColour *= attenuation;
		lightColour *= pointLights[i].intensity;

		// Specular power algorithm
		// Caulculate the shininess of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= pointLights[i].intensity;

		// Add our light values together to get the results
		result += lightColour + specular;
	}

	// ------------ SPOT LIGHTS
	for (int i = 0; i < addedSpotLights; ++i) {
		// Light direction from the point light to the vertex
		vec3 lightDir = normalize(spotLights[i].position - fVertPos);

		// Get spot light values
		// Found spot light implementation code from:
		// LearnOpenGL 2024, Light Casters, viewed August 18, https://learnopengl.com/Lighting/Light-casters
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		float epsilon = spotLights[i].innerCutOff - spotLights[i].outerCutOff;
		float spotLightIntensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);

		// Get the reflection light value
		vec3 reflectDir = reflect(-lightDir, normals);

		// How much light should show colour based on direction of normal facing the light
		float diff = max(dot(normals, lightDir), 0.0f);

		// Distance between the lights position and vertex position
		float distance = length(spotLights[i].position - fVertPos);

		// Actual attenuation calculation
		float attenCalc = 1.0f 
			+ spotLights[i].linear * distance 
			+ spotLights[i].quadratic * (distance * distance);
		
		// Distance that the light can reach
		// Value between 1 and 0, 1 is full light and 0 is no light
		float attenuation = 0.0f;

		// Ensure no division by 0
		if (attenCalc != 0.0f) {
			attenuation = 1.0f / attenCalc;
		}

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * spotLights[i].colour;
		lightColour *= diff;
		lightColour *= attenuation;
		lightColour *= spotLights[i].intensity;
		lightColour *= spotLightIntensity;

		// Specular power algorithm
		// Caulculate the shininess of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= spotLights[i].intensity * spotLightIntensity;
		specular *= spotLightIntensity;

		// Add our light values together to get the results
		result += lightColour + specular;
	}
	
	finalColour = vec4(result * brightness, 1.0f);
}