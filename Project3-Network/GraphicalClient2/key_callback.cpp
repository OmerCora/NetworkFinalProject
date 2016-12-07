#include "GlobalStuff.h"

#include <sstream>


bool isShiftDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
	{
		return true;
	}
	return false;
}

bool isCrtlDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
	{
		return true;
	}
	return false;
}

bool isAltDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
	{
		return true;
	}
	return false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

	switch (key)
	{
	// Light WSAD, QE, but for the light...
	case GLFW_KEY_A:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.x -= g_CameraMovementSpeed; }
		else										
		{
			::g_TargetX -= g_CameraMovementSpeed;
		}
		//{ ::g_CameraX -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_D:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.x += g_CameraMovementSpeed; }
		else										
		{ ::g_TargetX += g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_W:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.z -= g_CameraMovementSpeed; }
		else										
		{
			glm::vec3 direction = glm::normalize(glm::vec3(g_TargetX - g_CameraX, 0.0f, g_TargetZ - g_CameraZ));
			direction *= g_CameraMovementSpeed;
			::g_TargetZ += direction.z;
			::g_TargetX += direction.x;
		}
		//{ ::g_TargetZ -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_S:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.z += g_CameraMovementSpeed; }
		else	
			{
				glm::vec3 direction = glm::normalize(glm::vec3(g_TargetX - g_CameraX, 0.0f, g_TargetZ - g_CameraZ));
				direction *= g_CameraMovementSpeed;
				::g_TargetZ -= direction.z;
				::g_TargetX -= direction.x;
			}
			//::g_TargetZ += g_CameraMovementSpeed; 
		break;
	case GLFW_KEY_Q:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.y -= g_CameraMovementSpeed; }
		else										
		{ ::g_TargetY -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_E:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.y += g_CameraMovementSpeed; }
		else										
		{ ::g_TargetY += g_CameraMovementSpeed; }
		break;
	// Adjust lights attenuation
	case GLFW_KEY_1:
		if ( isShiftDownAlone(mods) ) {	::lightLinearAtten *= 0.99f; }	// Drop it by 1%
		break;
	case GLFW_KEY_2:
		if ( isShiftDownAlone(mods) ) {	::lightLinearAtten *= 1.01f;}	// Increase by 1%
		break;
	case GLFW_KEY_3:
		if ( isShiftDownAlone(mods) ) {	::lightQuadAtten *= 0.99f;}	
		break;
	case GLFW_KEY_4:
		if ( isShiftDownAlone(mods) ) {	::lightQuadAtten *= 1.01f;}	
		break;


	}//switch (key)




	// Update the camera position in the title...
//	glfwSetWindowTitle( window, "Sexy" );

	std::stringstream ssTitle;
	ssTitle << "Camera: " 
		<< ::g_CameraX << ", "
		<< ::g_CameraY << ", "
		<< ::g_CameraZ
		<< "  Light: " 
		<< ::lightPos.x << ", " 
		<< ::lightPos.y << ", " 
		<< ::lightPos.z
		<< " Lin: " << ::lightLinearAtten
		<< " Quad: " << ::lightQuadAtten;
	glfwSetWindowTitle( window, ssTitle.str().c_str() );


	return;
}
