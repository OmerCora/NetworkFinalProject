#include "GlobalStuff.h"

void LightShaderInit()
{
	locID_lightPosition[0] = glGetUniformLocation(shadProgID, "theLights[0].position");
	locID_lightDiffuse[0] = glGetUniformLocation(shadProgID, "theLights[0].diffuse");
	locID_lightAttenuation[0] = glGetUniformLocation(shadProgID, "theLights[0].attenuation");

	locID_lightPosition[1] = glGetUniformLocation(shadProgID, "theLights[1].position");
	locID_lightDiffuse[1] = glGetUniformLocation(shadProgID, "theLights[1].diffuse");
	locID_lightAttenuation[1] = glGetUniformLocation(shadProgID, "theLights[1].attenuation");

	locID_lightPosition[2] = glGetUniformLocation(shadProgID, "theLights[2].position");
	locID_lightDiffuse[2] = glGetUniformLocation(shadProgID, "theLights[2].diffuse");
	locID_lightAttenuation[2] = glGetUniformLocation(shadProgID, "theLights[2].attenuation");

	locID_lightPosition[3] = glGetUniformLocation(shadProgID, "theLights[3].position");
	locID_lightDiffuse[3] = glGetUniformLocation(shadProgID, "theLights[3].diffuse");
	locID_lightAttenuation[3] = glGetUniformLocation(shadProgID, "theLights[3].attenuation");

	locID_lightPosition[4] = glGetUniformLocation(shadProgID, "theLights[4].position");
	locID_lightDiffuse[4] = glGetUniformLocation(shadProgID, "theLights[4].diffuse");
	locID_lightAttenuation[4] = glGetUniformLocation(shadProgID, "theLights[4].attenuation");

	locID_lightPosition[5] = glGetUniformLocation(shadProgID, "theLights[5].position");
	locID_lightDiffuse[5] = glGetUniformLocation(shadProgID, "theLights[5].diffuse");
	locID_lightAttenuation[5] = glGetUniformLocation(shadProgID, "theLights[5].attenuation");

	locID_lightPosition[6] = glGetUniformLocation(shadProgID, "theLights[6].position");
	locID_lightDiffuse[6] = glGetUniformLocation(shadProgID, "theLights[6].diffuse");
	locID_lightAttenuation[6] = glGetUniformLocation(shadProgID, "theLights[6].attenuation");

	locID_lightPosition[7] = glGetUniformLocation(shadProgID, "theLights[7].position");
	locID_lightDiffuse[7] = glGetUniformLocation(shadProgID, "theLights[7].diffuse");
	locID_lightAttenuation[7] = glGetUniformLocation(shadProgID, "theLights[7].attenuation");

	locID_lightPosition[8] = glGetUniformLocation(shadProgID, "theLights[8].position");
	locID_lightDiffuse[8] = glGetUniformLocation(shadProgID, "theLights[8].diffuse");
	locID_lightAttenuation[8] = glGetUniformLocation(shadProgID, "theLights[8].attenuation");

	locID_lightPosition[9] = glGetUniformLocation(shadProgID, "theLights[9].position");
	locID_lightDiffuse[9] = glGetUniformLocation(shadProgID, "theLights[9].diffuse");
	locID_lightAttenuation[9] = glGetUniformLocation(shadProgID, "theLights[9].attenuation");

	locID_lightPosition[10] = glGetUniformLocation(shadProgID, "theLights[10].position");
	locID_lightDiffuse[10] = glGetUniformLocation(shadProgID, "theLights[10].diffuse");
	locID_lightAttenuation[10] = glGetUniformLocation(shadProgID, "theLights[10].attenuation");

	locID_lightPosition[11] = glGetUniformLocation(shadProgID, "theLights[11].position");
	locID_lightDiffuse[11] = glGetUniformLocation(shadProgID, "theLights[11].diffuse");
	locID_lightAttenuation[11] = glGetUniformLocation(shadProgID, "theLights[11].attenuation");
}

void LightSetting()
{
	glUniform4f(locID_lightPosition[0], -20.0f, lightPos.y, 0.0f, 1.0f);
	glUniform4f(locID_lightDiffuse[0], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[0], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[1], g_pValueReader->LightPositionX[1], lightPos.y, g_pValueReader->LightPositionZ[1], 1.0f);
	glUniform4f(locID_lightDiffuse[1], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[1], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[2], g_pValueReader->LightPositionX[2], lightPos.y, g_pValueReader->LightPositionZ[2], 1.0f);
	glUniform4f(locID_lightDiffuse[2], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[2], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[3], g_pValueReader->LightPositionX[3], lightPos.y, g_pValueReader->LightPositionZ[3], 1.0f);
	glUniform4f(locID_lightDiffuse[3], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[3], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[4], g_pValueReader->LightPositionX[4], lightPos.y, g_pValueReader->LightPositionZ[4], 1.0f);
	glUniform4f(locID_lightDiffuse[4], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[4], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[5], g_pValueReader->LightPositionX[5], lightPos.y, g_pValueReader->LightPositionZ[5], 1.0f);
	glUniform4f(locID_lightDiffuse[5], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[5], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[6], g_pValueReader->LightPositionX[6], lightPos.y, g_pValueReader->LightPositionZ[6], 1.0f);
	glUniform4f(locID_lightDiffuse[6], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[6], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[7], g_pValueReader->LightPositionX[7], lightPos.y, g_pValueReader->LightPositionZ[7], 1.0f);
	glUniform4f(locID_lightDiffuse[7], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[7], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[8], g_pValueReader->LightPositionX[8], lightPos.y, g_pValueReader->LightPositionZ[8], 1.0f);
	glUniform4f(locID_lightDiffuse[8], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[8], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[9], g_pValueReader->LightPositionX[9], lightPos.y, g_pValueReader->LightPositionZ[9], 1.0f);
	glUniform4f(locID_lightDiffuse[9], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[9], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[10], g_pValueReader->LightPositionX[10], lightPos.y, g_pValueReader->LightPositionZ[10], 1.0f);
	glUniform4f(locID_lightDiffuse[10], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[10], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

	glUniform4f(locID_lightPosition[11], g_pValueReader->LightPositionX[11], lightPos.y, g_pValueReader->LightPositionZ[11], 1.0f);
	glUniform4f(locID_lightDiffuse[11], 1.0, 1.0f, 1.0f, 1.0f);
	glUniform4f(locID_lightAttenuation[11], lightConstAtten, lightLinearAtten, lightQuadAtten, 1.0f);

}