#version 400

// FRAGMENT shader

in vec4 diffuseColour;
in vec4 vertWorldXYZ;
in vec4 vertNorm;

uniform vec3 myLightPosition;		// In the world
uniform vec3 myLightDiffuse;		// 1,1,1 (white colour light)

out vec4 out_Colour;

void main()
{
	vec3 lightv = myLightPosition - vertWorldXYZ.xyz;
	
	lightv = normalize(lightv);
	
 	vec3 diffuse = max(0.0, dot(lightv, vertNorm.xyz)) 
                   * diffuseColour.xyz
				   * myLightDiffuse;
				   //* vec3(1.0f,0.0f,0.0f);
							
	// With newer (we're using version 4 of GLSL) some of the 
	// 	built in variables are depricated, like gl_FragColor
	//gl_FragColor = vec4(diffuse, 1.0f);	

	vec3 ambientColour = vec3(0.14f,0.14f,0.14f) * diffuseColour.xyz;
	out_Colour = vec4(diffuse + ambientColour, 1.0f);	

	//out_Colour *= 0.001f;	// make it (essentially) zero.	
	//out_Colour += vec4(diffuseColour.xyz, 1.0f); //vec4(1,1,1,1);
}