#ifndef _RENDERINFO_HG_
#define _RENDERINFO_HG_

#include "externals.h"

#include "GelStructures.h"

class RenderInfo
{
public:
	RenderInfo();
	~RenderInfo();

	bool useDebugColorsNoLighting;
	bool isDrawn;
	bool isWireframe;
	bool isAABBDrawn;
	bool useTextureAsDiffuse;
	bool discardTextureBlack;

	glm::mat4x4 transform;

	glm::vec4 color;
	float alpha;
	void ToggleColor(glm::vec4 color);
	void ResetToggleColor(void);

	GelRotationPre rotationPre;
	//GelRotation rotation;
	GelOrientation orientation; // glm::quat

	//these all might move to physics?

	GelPosition position;
	GelScale scale; //float
	GelScale lastDrawnActualScale;

	float GetRadius(void);
	void SetRadius(float newRadius);
	float AABBwidth;
private:
	float radius;

	glm::vec4 toggle_color;
};

#endif // !_RENDERINFO_HG_
