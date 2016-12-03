#include "RenderInfo.h"


RenderInfo::RenderInfo()
{
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->scale = 1.0f;
	this->lastDrawnActualScale = 1.0f;
	this->alpha = 1.0f;
	this->isDrawn = true;
	this->isAABBDrawn = false;
	this->AABBwidth = 1.0f;
	this->isWireframe = false;
	this->useDebugColorsNoLighting = false;
	this->useTextureAsDiffuse = true;
	this->discardTextureBlack = false;
	this->position.x = this->position.y = this->position.z = 0.0f;
	//this->rotation.x = this->rotation.y = this->rotation.z = 0.0f;
	this->rotationPre.x = this->rotationPre.y = this->rotationPre.z = 0.0f;
}

RenderInfo::~RenderInfo()
{
}

void RenderInfo::ToggleColor(glm::vec4 newColor)
{
	if (newColor == this->color)
	{
		return; //and do nothing
	}
	this->toggle_color = this->color;
	this->color = newColor;
}

void RenderInfo::ResetToggleColor(void)
{
	this->color = this->toggle_color;
}

float RenderInfo::GetRadius(void)
{
	// 1% increased radius for more tolerant broad filter
	return radius * scale * 1.0f;
}

void RenderInfo::SetRadius(float newRadius)
{
	this->radius = newRadius;
}
