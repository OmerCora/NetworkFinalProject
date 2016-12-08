#version 400

// FRAGMENT shader

uniform bool bUseObjColorNoTexture;

const int NUMBEROFLIGHTS = 12;

struct sLightDesc
{
	vec4 position;		// In the world
	vec4 diffuse;		
	vec4 attenuation;	
	vec4 specular;		
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

uniform vec3 eyeLocation;

uniform float alpha;		

uniform samplerCube myCubemapSkyBoxSampler;
uniform sampler2D texSamp2D_00;	
uniform bool bIsCubeMapObject;

uniform bool bUseTextureAsDiffuse;
uniform bool bUseLighting;
uniform bool bIsImposterBlack;

in vec4 diffuseColour;	
in vec3 vertMV;			
in vec3 vertWorld;
in vec3 vertNorm;

in vec3 lightVector[NUMBEROFLIGHTS];
in vec3 lightHalfVector[NUMBEROFLIGHTS];
in vec3 text_STU;	


out vec4 out_Colour; 


vec3 calcLightContribution( vec3 diffuseObjectColour, vec3 vecView, int lightIndex )
{
	float distanceFromLight = length(lightVector[lightIndex].xyz); 
	float atten = 1.0f /( theLights[lightIndex].attenuation.x + 
	                      theLights[lightIndex].attenuation.y * distanceFromLight +
						  theLights[lightIndex].attenuation.z * distanceFromLight * distanceFromLight );
	vec3 lightVec = normalize(lightVector[lightIndex].xyz);
 	vec3 diffuse = max(0.0, dot(vertNorm.xyz, lightVec)) * diffuseObjectColour;	
	diffuse *= atten;	
	vec3 viewVector = normalize(eyeLocation - vertWorld);
	vec3 vecReflect = normalize(reflect(-lightVec,vertNorm));
	float specReflect = max(dot(vecReflect, viewVector), 0.0f);	
	float lightSpecShinniness = theLights[lightIndex].specular.w;
	float specPower = pow( specReflect, lightSpecShinniness );
	vec3 specular = specPower * theLights[lightIndex].specular.xyz;
	specular *= atten;
	specular = clamp(specular, 0.0f, 1.0f);
	vec3 result = clamp(diffuse + specular, 0.0, 1.0f);	
	return result;
}

void main()
{	

	if ( bUseObjColorNoTexture )
	{	
		out_Colour.xyz = diffuseColour.xyz;
		out_Colour.a = alpha;
		return;
	}

	
		if ( bIsCubeMapObject )
	{
		out_Colour =    texture( myCubemapSkyBoxSampler, vertNorm.xyz );
		out_Colour.a = 1.0f;
		return;
	}

	vec3 objectDiffuseColour = diffuseColour.xyz;

	float alphaOut = alpha;

	if ( bUseTextureAsDiffuse )
	{			
		vec3 diffuse0 = texture( texSamp2D_00, text_STU.xy ).rgb;
		
		objectDiffuseColour *= diffuse0;		
		alphaOut = normalize(diffuse0.r + diffuse0.g + diffuse0.b );
	}
	
	if ( ! bUseLighting )
	{	
		out_Colour = vec4(objectDiffuseColour, alphaOut);	
	}
	else
	{	
		vec3 vecView = normalize(vertMV);

		vec3 lightContrib = vec3(0.0f, 0.0f, 0.0f);
		for ( int index = 0; index != NUMBEROFLIGHTS; index++ )
		{
			lightContrib += calcLightContribution( objectDiffuseColour, vecView, index );
		}
		
		out_Colour = vec4(lightContrib, alphaOut);	
	}

	if ( bIsImposterBlack)
	{
		if (( (out_Colour.r + out_Colour.g + out_Colour.b)/3.0f) < 0.2f )
		{	
			discard;
		}
	}	
}