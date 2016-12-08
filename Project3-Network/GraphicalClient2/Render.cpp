#include "GlobalStuff.h"
#include "Utilities.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//#include "cGameObject.h"



void SetUpTextureBindings(cEntity* pCurrEntity)
{
	// Get texture number
	std::string textureName = pCurrEntity->mRenderingInfo.textureName;

	GLuint texture00Number = ::g_pTextureManager->getTextureIDFromName(textureName);

	if (texture00Number > 0)
	{
		glUniform1i(UniformLoc_bUseTextureAsDiffuse, TRUE);
		GLuint texture00Unit = 1;

		glActiveTexture(texture00Unit + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture00Number);
		glUniform1i(UniformLoc_texSamp2D_00, texture00Unit);

	}

	return;
}



void DrawObject(cEntity* pCurrEntity, bool bUseLighting /*= true*/ )
{
	//if ( ! pCurrEntity->bIsVisible )
	//{
	//	// Don't draw it
	//	return;
	//}

	std::string meshModelName = pCurrEntity->mRenderingInfo.meshName;
			
	GLuint VAO_ID = 0;
	int numberOfIndices = 0;
	float unitScale = 1.0f;
	if ( ! ::g_pTheMeshTypeManager->LookUpMeshInfo( meshModelName,
													VAO_ID, 
													numberOfIndices,
													unitScale ) )
	{	// Skip the rest of the for loop, but continue
		return;
	}


	glCullFace( GL_BACK );		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
	glEnable( GL_CULL_FACE );
	glPolygonMode( GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_FILL );			// GL_POINT, GL_LINE, or GL_FILL


	SetUpTextureBindings( pCurrEntity );
	

	if ( bUseLighting )
	{
		glUniform1i( UniformLoc_bUseLighting, TRUE );
	}
	else 
	{		
		glUniform1i( UniformLoc_bUseLighting, FALSE );
	}
			
	::g_pTheShaderManager->useShaderProgram("simple");


	//glm::mat4x4(1.0f);
	//matModel = glm::translate(matModel, pCurrEntity->mPhysicsInfo.Position);
	//matModel = matModel * glm::mat4_cast(pCurrEntity->mPhysicsInfo.Orientation);
	//glm::vec3 actualScale = unitScale * pCurrEntity->mRenderingInfo.Scale;
	pCurrEntity->preRenderer();
	glm::mat4x4 matModel = pCurrEntity->mRenderingInfo.modelMatrix;
	glm::vec3 actualScale = unitScale * pCurrEntity->mRenderingInfo.Scale;
	matModel = glm::scale(matModel, actualScale);

	glUniformMatrix4fv( locID_matModel, 1, GL_FALSE, 
				        (const GLfloat*) glm::value_ptr(matModel) );

	glUniform4f( UniformLoc_ID_objectColour, 1.0f, 1.0f, 1.0f, 1.0f );

		glUniform1i( UniformLoc_ID_isWireframe, FALSE );	
		glUniform1i( UniformLoc_ID_bUseObjColorNoTexture, FALSE );	
		glUniform1i(UniformLoc_bIsImposterBlack, FALSE);

	// "turns on" transparency
	glEnable( GL_BLEND );		// Enables "blending"
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


	// Set transparency
	glUniform1f( UniformLoc_alpha, 1.0f/*pCurrEntity->alpha*/ );



	glBindVertexArray( VAO_ID );
	glDrawElements( GL_TRIANGLES, 
					numberOfIndices, 
					GL_UNSIGNED_INT,	// Each index is how big?? 
					(GLvoid*) 0 );		// Starting point in buffer
	glBindVertexArray(0);

	return;
}


