#ifndef _OCHELPER_HG_
#define _OCHELPER_HG_

#include "externals.h"

class OCHelper
{
public:
	OCHelper();
	~OCHelper();

	static float getRandFloat(float LO, float HI);
	static float clamp(float x, float min, float max);
	static float smootherStep(float min, float max, float x);
	static void calculateNormalOfTriangle(glm::vec3 vertex_1, glm::vec3 vertex_2, glm::vec3 vertex_3, glm::vec3& normalVector);
private:

};


#endif // !_OCHELPER_HG_
