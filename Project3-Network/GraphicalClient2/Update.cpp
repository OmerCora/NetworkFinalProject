#include "Update.h"
#include <iostream>


//update the scene and supplying delta time to camera and entities
void Update()
{
	double tmpLastTime = glfwGetTime();
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	if (deltaTime == 0.f)
	{
		lastTime = currentTime;
		return;
	}
	if (deltaTime < 0.01)
	{
		return;
	}
	if (deltaTime > 0.05)
	{
		deltaTime = 0.05;
	}

	for (auto itr = gVec_Entities.begin(); itr != gVec_Entities.end(); itr++)
	{
		if (!(*itr)->mPhysicsInfo.IsStatic)
		{

			(*itr)->Update(deltaTime);


			// When reaches boundaries, their positions will be at the other side 
			//   and their states will change according to the position of the player
			if ((*itr)->mPhysicsInfo.Position.x > gAreaInfo->Max.x)
			{				
				(*itr)->mbForwardable = false;
				if ((*itr)->GetForward().x < 0)
					(*itr)->mbForwardable = true;

			}
			else if ((*itr)->mPhysicsInfo.Position.z > gAreaInfo->Max.z)
			{
				(*itr)->mbForwardable = false;	
				if ((*itr)->GetForward().z < 0)
					(*itr)->mbForwardable = true;
			}
			else if ((*itr)->mPhysicsInfo.Position.x < gAreaInfo->Min.x)
			{
				(*itr)->mbForwardable = false;
				if ((*itr)->GetForward().x > 0)
					(*itr)->mbForwardable = true;
			}
			else if ((*itr)->mPhysicsInfo.Position.z < gAreaInfo->Min.z)
			{
				(*itr)->mbForwardable = false;
				if ((*itr)->GetForward().z > 0)
					(*itr)->mbForwardable = true;
			}
			else
				(*itr)->mbForwardable = true;

			

		}//if (!(*itr)->mPhysicsInfo.IsStatic)



    }//	for (auto itr = gVec_Entities.begin(); itr != gVec_Entities.end(); itr++)

	gCamera->Update(deltaTime);

	lastTime = currentTime;

}


//glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
//{
//	glm::vec3 ab = b - a;
//	glm::vec3 ac = c - a;
//	glm::vec3 ap = p - a;
//	float d1 = glm::dot(ab, ap);
//	float d2 = glm::dot(ac, ap);
//	if (d1 <= 0.0f && d2 <= 0.0f) return a;
//
//
//	glm::vec3 bp = p - b;
//	float d3 = glm::dot(ab, bp);
//	float d4 = glm::dot(ac, bp);
//	if (d3 >= 0.0f && d4 <= d3) return b;
//
//
//	float vc = d1*d4 - d3*d2;
//	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
//		float v = d1 / (d1 - d3);
//		return a + v * ab;
//	}
//
//	glm::vec3 cp = p - c;
//	float d5 = glm::dot(ab, cp);
//	float d6 = glm::dot(ac, cp);
//	if (d6 >= 0.0f && d5 <= d6) return c;
//
//	float vb = d5*d2 - d1*d6;
//	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
//		float w = d2 / (d2 - d6);
//		return a + w * ac;
//	}
//
//	float va = d3*d6 - d5*d4;
//	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
//		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
//		return b + w * (c - b);
//	}
//
//	float denom = 1.0f / (va + vb + vc);
//	float v = vb * denom;
//	float w = vc * denom;
//	return a + ab * v + ac * w;
//}