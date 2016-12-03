#include "gelRenderManager.h"

#include "GelsenizeCore.h"
#include "externals.h"
#include "managers.h"
#include "iGameObject.h"

#include "cAABB.h"

#include <algorithm>

#include "cContact.h"

#include "cFlock.h"

//void GelRenderManager::DrawScene(void)
//{
//	return;
//}

void gelRenderManager::Update(float deltaTime)
{
	//draw each game object
	//DrawPlayer(); //this is now done in DrawGameObjects

	DrawSkyboxObject();

	DrawGameObjects();

	std::vector<cContact> contactVec;
	GelCollisionManager::sharedInstance().GetContacts(contactVec);

	for (int i = 0; i != contactVec.size(); i++)
	{
		cContact contact = contactVec[i];

		MeshInfo mesh;
		CollisionInfo collisionInfo;
		mesh.name = "sphere";
		RenderInfo render;

		render.position = contact.contactXYZ;
		render.scale = 0.1f;
		render.isWireframe = true;
		//render.isAABBDrawn = true;
		//render.AABBwidth = 10.0f;
		render.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		render.useDebugColorsNoLighting = true;

		this->DrawWithInfos(render, mesh, collisionInfo);
	}

	//DrawParticleEmitter();
}

void gelRenderManager::Setup(void)
{
	// Add a "debug AABB"
	this->aabb = gelObjectFactory::sharedInstance().createGameObject("object");

	MeshInfo mesh;
	mesh.name = "aabb_cube";

	RenderInfo render;
	render.scale = 1.0f;

	this->aabb->SetMesh(mesh);
	this->aabb->SetRender(render);
	
	//TODO: do we do this? hmm
	//gelObjectManager::sharedInstance().AddGameObject(aabb);
}

void gelRenderManager::DrawPlayer(void)
{
	globalAlpha = 1.0f;
	useGlobalAlpha = false;
	
	iGameObject *player = gelObjectManager::sharedInstance().GetMainPlayer();
	this->DrawGameObject(player);
}

void gelRenderManager::DrawGameObjects(void)
{
	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();

	glm::vec4 eyePosition0;
	gelCameraManager::sharedInstance().mainCamera->GetEyePosition(eyePosition0);
	glm::vec3 eyePosition = glm::vec3(eyePosition0.x, eyePosition0.y, eyePosition0.z);

	//reorder by eye vector
	struct CompStruct
	{
		int objectID;
		float distanceToEye;

		CompStruct(int id, float s) : objectID(id), distanceToEye(s) {}
	};

	struct less_than_key
	{
		inline bool operator() (const CompStruct& struct1, const CompStruct& struct2)
		{
			return (struct1.distanceToEye < struct2.distanceToEye);
		}
	};

	std::vector<CompStruct> compVector;

	for (int index = 0; index != gameObjects.size(); index++)
	{
		glm::vec3 objPos = gameObjects[index]->GetRender().position;

		compVector.push_back(CompStruct(gameObjects[index]->getObjectID(), glm::distance(eyePosition, objPos)));

		/*if (gameObjects[index]->ObjectType() == "particleEmitter")
		{
			this->DrawParticleEmitter((cParticleEmitter*)gameObjects[index]);
		}
		else
		{
			this->DrawGameObject(gameObjects[index]);
		}
		*/
	}
	std::sort(compVector.begin(), compVector.end(), less_than_key());

	for (int index = 0; index != compVector.size(); index++)
	{
		CompStruct compStruct = compVector[index];
		iGameObject* gameObject = gelObjectManager::sharedInstance().FindGameObjectWithID(compStruct.objectID);
		if (gameObject->ObjectType() == "particleEmitter")
		{
			this->DrawParticleEmitter((cParticleEmitter*)gameObject);
		}
		else if (gameObject->ObjectType() == "flock")
		{
			std::vector<iGameObject*> flockMembers;
			((cFlock*)gameObject)->GetMembers(flockMembers);
			for (int k = 0; k != flockMembers.size(); k++)
			{
				DrawGameObject(flockMembers[k]);
			}
		}
		else
		{
			this->DrawGameObject(gameObject);
		}
	}
}

void gelRenderManager::DrawParticleEmitter(cParticleEmitter *emitter)
{

	glm::vec4 eyePos;
	gelCameraManager::sharedInstance().mainCamera->GetEyePosition(eyePos);
	emitter->particleSettings.imposterEyeLocation.x = eyePos.x;
	emitter->particleSettings.imposterEyeLocation.y = eyePos.y;
	emitter->particleSettings.imposterEyeLocation.z = eyePos.z;

	std::vector<cParticle> vecParticles;
	gelObjectManager::sharedInstance().GetParticleVectorToRender(emitter->getObjectID(), vecParticles);

	int numParticles = emitter->GetParticlesCopy(vecParticles);
	for (int i = 0; i != numParticles; i++)
	{
		DrawParticle(vecParticles[i]);
	}

	//std::vector<cParticleEmitter*> particleEmitters = gelObjectManager::sharedInstance().GetParticleEmitters();
	//for (int index = 0; index != particleEmitters.size(); index++)
	//{

	//	glm::vec4 eyePos;
	//	gelCameraManager::sharedInstance().mainCamera->GetEyePosition(eyePos);
	//	cParticleEmitter *emitter = particleEmitters[index];
	//	emitter->particleSettings.imposterEyeLocation.x = eyePos.x;
	//	emitter->particleSettings.imposterEyeLocation.y = eyePos.y;
	//	emitter->particleSettings.imposterEyeLocation.z = eyePos.z;

	//	std::vector<cParticle> vecParticles;
	//	gelObjectManager::sharedInstance().GetParticleVectorToRender(index, vecParticles);

	//	int numParticles = emitter->GetParticlesCopy(vecParticles);
	//	for (int i = 0; i != numParticles; i++)
	//	{
	//		DrawParticle(vecParticles[i]);
	//	}


	//	//this->DrawGameObject(particleEmitters[index]);
	//}
}

void gelRenderManager::DrawWithInfos(RenderInfo render, MeshInfo mesh, CollisionInfo collision)
{
	glm::mat4x4 m = glm::mat4x4(1.0f);
	glm::mat4x4 v; gelCameraManager::sharedInstance().mainCamera->GetViewMatrix(v);
	glm::mat4x4 p; gelCameraManager::sharedInstance().mainCamera->GetProjectionMatrix(p);

	std::string meshModelName = mesh.name;

	GLuint VAO_ID = 0;
	int numberOfIndices = 0;
	float unitScale = 1.0f;
	bool draw = render.isDrawn;
	if (!gelMeshManager::sharedInstance().LookUpMeshInfo(meshModelName, VAO_ID, numberOfIndices, unitScale))
	{
		//could not find the mesh info
		draw = false;
	}

	m = glm::translate(m, glm::vec3(render.position.x, render.position.y, render.position.z));

	//m = glm::rotate(m, render.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//m = glm::rotate(m, render.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//m = glm::rotate(m, render.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matRotation(render.orientation);
	m = m * matRotation;

	float actualScale = render.scale * unitScale;
	m = glm::scale(m, glm::vec3(actualScale, actualScale, actualScale));
	render.transform = m;
	//render.lastDrawnActualScale = actualScale;
	//pCurGO->SetRender(render);

	//draw aabb for current game object
	if (render.isAABBDrawn)
	{
		DrawAABB(
			glm::vec3(render.position.x, render.position.y, render.position.z),
			render.color,
			render.AABBwidth);
	}

	if (draw)
	{
		if (render.isWireframe)
		{	// Turn off backface culling
			// Enable "wireframe" polygon mode
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
			glDisable(GL_CULL_FACE);
		}
		else
		{	// "Regular" rendering: 
			// Turn on backface culling
			// Turn polygon mode to solid (Fill)
			glCullFace(GL_BACK);		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
		}

		if (render.useTextureAsDiffuse)
		{
			GelTextureManager::sharedInstance().BindTexture(mesh.textureName_0, 0);
			glUniform1i(GelsenizeCore::sharedInstance().UseTextureAsDiffuseID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseTextureAsDiffuseID(), FALSE);
		}

		if (render.discardTextureBlack)
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), FALSE);
		}

		//mvp = p * v * m;

		glUniform1i(GelsenizeCore::sharedInstance().IsImposterBlackIsAlphaID(), FALSE);

		if (1)
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}


		gelShaderManager::sharedInstance().useShaderProgram("default");
		glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetModelLocation(), 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(m));
		glUniform4fv(GelsenizeCore::sharedInstance().ModelColorID(), 1, glm::value_ptr(render.color));
		glUniform1i(GelsenizeCore::sharedInstance().isWireframeID(), render.isWireframe);
		glUniform1i(GelsenizeCore::sharedInstance().UseGlobalAlphaValue(), this->useGlobalAlpha);
		glUniform1f(GelsenizeCore::sharedInstance().GlobalAlphaValue(), this->globalAlpha);

		glUniform1i(GelsenizeCore::sharedInstance().UsedTexNumID(), mesh.usedTextureNum);

		// "turns on" transparency
		glEnable(GL_BLEND);		// Enables "blending"
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1f(GelsenizeCore::sharedInstance().ObjectAlphaID(), render.alpha);

		//added
		if (render.useDebugColorsNoLighting)
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), TRUE); // 1
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), FALSE);	// 0
		}

		glBindVertexArray(VAO_ID);
		glDrawElements(GL_TRIANGLES,
			numberOfIndices,
			GL_UNSIGNED_INT,	// Each index size 
			(GLvoid*)0);		// Starting point in buffer
		glBindVertexArray(0);
	}
}

void gelRenderManager::DrawGameObject(iGameObject* gameObject)
{
	//glm::mat4x4 mvp(1.0f);		// Model, View, Projection matrix
	glm::mat4x4 m = glm::mat4x4(1.0f);
	glm::mat4x4 v; gelCameraManager::sharedInstance().mainCamera->GetViewMatrix(v);
	glm::mat4x4 p; gelCameraManager::sharedInstance().mainCamera->GetProjectionMatrix(p);

	iGameObject* pCurGO = gameObject;
	RenderInfo render = pCurGO->GetRender();
	MeshInfo mesh = pCurGO->GetMesh();

	std::string meshModelName = mesh.name;

	GLuint VAO_ID = 0;
	int numberOfIndices = 0;
	float unitScale = 1.0f;
	bool draw = render.isDrawn;
	if (!gelMeshManager::sharedInstance().LookUpMeshInfo(meshModelName, VAO_ID, numberOfIndices, unitScale))
	{
		//could not find the mesh info
		draw = false;
	}

	m = glm::translate(m, glm::vec3(render.position.x, render.position.y, render.position.z));

	//m = glm::rotate(m, render.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//m = glm::rotate(m, render.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//m = glm::rotate(m, render.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matRotation(render.orientation);
	m = m * matRotation;
 
	float actualScale = render.scale * unitScale;
	m = glm::scale(m, glm::vec3(actualScale, actualScale, actualScale));
	render.transform = m;
	render.lastDrawnActualScale = actualScale;
	pCurGO->SetRender(render);

	//draw aabb for current game object
	if (render.isAABBDrawn)
	{
		DrawAABB(
			glm::vec3(render.position.x, render.position.y, render.position.z),
			render.color,
			render.AABBwidth); 
	}

	if (draw)
	{
		if (render.isWireframe)
		{	// Turn off backface culling
			// Enable "wireframe" polygon mode
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
			glDisable(GL_CULL_FACE);
		}
		else
		{	// "Regular" rendering: 
			// Turn on backface culling
			// Turn polygon mode to solid (Fill)
			glCullFace(GL_BACK);		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
		}

		if (render.useTextureAsDiffuse)
		{
			GelTextureManager::sharedInstance().BindTexture(mesh.textureName_0, 0);
			glUniform1i(GelsenizeCore::sharedInstance().UseTextureAsDiffuseID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseTextureAsDiffuseID(), FALSE);
		}

		if (render.discardTextureBlack)
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), FALSE);
		}

		//mvp = p * v * m;
		
		glUniform1i(GelsenizeCore::sharedInstance().IsImposterBlackIsAlphaID(), FALSE);

		if (1)
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}


		gelShaderManager::sharedInstance().useShaderProgram("default");
		glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetModelLocation(), 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(m));
		glUniform4fv(GelsenizeCore::sharedInstance().ModelColorID(), 1, glm::value_ptr(render.color));
		glUniform1i(GelsenizeCore::sharedInstance().isWireframeID(), render.isWireframe);
		glUniform1i(GelsenizeCore::sharedInstance().UseGlobalAlphaValue(), this->useGlobalAlpha);
		glUniform1f(GelsenizeCore::sharedInstance().GlobalAlphaValue(), this->globalAlpha);

		glUniform1i(GelsenizeCore::sharedInstance().UsedTexNumID(), mesh.usedTextureNum);

		// "turns on" transparency
		glEnable(GL_BLEND);		// Enables "blending"
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1f(GelsenizeCore::sharedInstance().ObjectAlphaID(), render.alpha);

		//added
		if (render.useDebugColorsNoLighting)
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), TRUE); // 1
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), FALSE);	// 0
		}

		glBindVertexArray(VAO_ID);
		glDrawElements(GL_TRIANGLES,
			numberOfIndices,
			GL_UNSIGNED_INT,	// Each index size 
			(GLvoid*)0);		// Starting point in buffer
		glBindVertexArray(0);

		//return;

		//finally if object collision debug mode allow draw spme spheres,
		CollisionInfo collisionInfo = pCurGO->GetCollision();
		if (collisionInfo.debug_mode != CollisionInfo::DEBUG_MODE::NO_DRAW)
		{
			//DRAW 
			if (collisionInfo.type == CollisionInfo::TYPE::SPHERE_ARRAY)
			{
				//try to access sphere array
				std::vector<CollisionInfo::CollisionSphere> sphereArray;
				if (GelCollisionManager::sharedInstance().GetSphereArrayForAlias(mesh.name, sphereArray))
				{
					for (int index = 0; index != sphereArray.size(); index++)
					{
						CollisionInfo::CollisionSphere sphere = sphereArray[index];
						glm::vec3 center_offset = sphere.center_offset;
						glm::vec3 objPosition = render.position;

						MeshInfo sphereMesh;
						sphereMesh.name = "sphere";
						RenderInfo sphereRender;

						//attempt to move sphere to its location
						m = glm::translate(render.transform, glm::vec3(center_offset.x, center_offset.y, center_offset.z));
						
						sphereRender.position = m[3];
						sphereRender.scale = actualScale * sphere.radius;
						sphereRender.isWireframe = true;
						sphereRender.isAABBDrawn = false;
						sphereRender.AABBwidth = actualScale * 10.0f;
						sphereRender.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
						sphereRender.useDebugColorsNoLighting = true;

						this->DrawWithInfos(sphereRender, sphereMesh, collisionInfo);
					}
					
				}
			}
		}
	}
	return;
}


void gelRenderManager::DrawParticle(cParticle particle)
{
	//glm::mat4x4 mvp(1.0f);		// Model, View, Projection matrix
	glm::mat4x4 m = glm::mat4x4(1.0f);
	glm::mat4x4 v; gelCameraManager::sharedInstance().mainCamera->GetViewMatrix(v);
	glm::mat4x4 p; gelCameraManager::sharedInstance().mainCamera->GetProjectionMatrix(p);

	cParticle pCurrentParticle = particle;
	//RenderInfo render = pCurGO.GetRender();
	std::string meshModelName = pCurrentParticle.meshName;

	GLuint VAO_ID = 0;
	int numberOfIndices = 0;
	float unitScale = 10.0f;
	bool draw = true;
	bool isWireframe = false; 
	if (!gelMeshManager::sharedInstance().LookUpMeshInfo(meshModelName, VAO_ID, numberOfIndices, unitScale))
	{
		//could not find the mesh info
		draw = false;
	}

	m = glm::translate(m, glm::vec3(pCurrentParticle.position.x, pCurrentParticle.position.y, pCurrentParticle.position.z));

	m = glm::rotate(m, pCurrentParticle.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m = glm::rotate(m, pCurrentParticle.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m = glm::rotate(m, pCurrentParticle.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 matRotation(pCurrentParticle.orientation);
	//m = m * matRotation;

	float actualScale = pCurrentParticle.scale * unitScale;
	m = glm::scale(m, glm::vec3(actualScale, actualScale, actualScale));
	//render.transform = m;
	//pCurGO.SetRender(render);

	//draw aabb for current game object
	if (0) //dont draw for particles
	{
		DrawAABB(
			glm::vec3(pCurrentParticle.position.x, pCurrentParticle.position.y, pCurrentParticle.position.z),
			pCurrentParticle.colour,
			10.0f);
	}

	if (draw) //draw
	{
		if (isWireframe)
		{	// Turn off backface culling
			// Enable "wireframe" polygon mode
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
			glDisable(GL_CULL_FACE);
		}
		else
		{	// "Regular" rendering: 
			// Turn on backface culling
			// Turn polygon mode to solid (Fill)
			glCullFace(GL_BACK);		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
				GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
		}
		//mvp = p * v * m;

		gelShaderManager::sharedInstance().useShaderProgram("default");
		glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetModelLocation(), 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(m));
		glUniform4fv(GelsenizeCore::sharedInstance().ModelColorID(), 1, glm::value_ptr(pCurrentParticle.colour));
		glUniform1i(GelsenizeCore::sharedInstance().isWireframeID(), isWireframe);
		glUniform1i(GelsenizeCore::sharedInstance().UseGlobalAlphaValue(), this->useGlobalAlpha);
		glUniform1f(GelsenizeCore::sharedInstance().GlobalAlphaValue(), this->globalAlpha);

		glUniform1i(GelsenizeCore::sharedInstance().UsedTexNumID(), 1);

		//added
		if (false)//render.useDebugColorsNoLighting
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), TRUE); // 1
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DebugColorID(), FALSE);	// 0
		}

		glUniform1i(GelsenizeCore::sharedInstance().IsImposterBlackIsAlphaID(), FALSE);
		
		GelTextureManager::sharedInstance().BindTexture(pCurrentParticle.textureName_0, 0);
		glUniform1i(GelsenizeCore::sharedInstance().UseTextureAsDiffuseID(), TRUE);

		if (1)
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().UseLightingID(), TRUE);
		}

		if (particle.discardTextureBlack)
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), TRUE);
		}
		else
		{
			glUniform1i(GelsenizeCore::sharedInstance().DiscardTextureBlackID(), FALSE);
		}

		glBindVertexArray(VAO_ID);
		glDrawElements(GL_TRIANGLES,
			numberOfIndices,
			GL_UNSIGNED_INT,	// Each index size 
			(GLvoid*)0);		// Starting point in buffer
		glBindVertexArray(0);
	}
	return;
}

void gelRenderManager::DrawSkyboxObject(void)
{
	// Draw skybox
	// Place the skybox at the same location as the camera 'eye'

	glm::vec4 cameraPosition;
	gelCameraManager::sharedInstance().mainCamera->GetEyePosition(cameraPosition);

	iGameObject* skyboxObject = gelObjectManager::sharedInstance().GetSkyboxObject();
	RenderInfo render = skyboxObject->GetRender();
	render.position.x = cameraPosition.x;
	render.position.y = cameraPosition.y;
	render.position.z = cameraPosition.z;
	skyboxObject->SetRender(render);

	//::g_pSkyBoxObject->x = ::g_CameraX;
	//::g_pSkyBoxObject->y = ::g_CameraY;
	//::g_pSkyBoxObject->z = ::g_CameraZ;
	// There are a number of other things you could do with this, for example:
	// Turn off the depth buffer, and the writing to the depth buffer, 
	//  then draw the sky box FIRST. Then turn on the depth buffer and 
	//  continue to draw the rest of the scene. 
	// This case is more straight-forward, in that we simply make the sky box
	//  an appropriate size for the sky (so big) and be careful not to 
	//  draw anything "behind" it.

	glUniform1i(GelsenizeCore::sharedInstance().IsCubeMapObjectID(), TRUE);

	// Bind the appriprate texture unit... 
	GLuint texNumberSky = GelTextureManager::sharedInstance().GetTextureIDFromName("skybox");
	// We'll pick a higher value texture unit. 
	// There are at least 80 of them, so we'll pick the last one...
	// (Note the GL_TEXTUREX values only go up to 31, though)
	const unsigned int TEXTURE_UNIT_80_MAGIC_NUMBER = 80;
	glActiveTexture(TEXTURE_UNIT_80_MAGIC_NUMBER + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
																		//	glBindTexture( GL_TE, texNumberSky );
	glBindTexture(GL_TEXTURE_2D, texNumberSky);

	glUniform1i(GelsenizeCore::sharedInstance().CubemapSkyBoxSamplerID(), texNumberSky);

	glDisable(GL_DEPTH);			// turn off depth test (i.e. just write)
	glDepthMask(GL_FALSE);		// no writing to z or depth buffer
	DrawGameObject(skyboxObject);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH);

	glUniform1i(GelsenizeCore::sharedInstance().IsCubeMapObjectID(), FALSE);

	return;
}

int gelRenderManager::getAABBID(void)
{
	return this->aabbID;
}

void gelRenderManager::DrawAABB(glm::vec3 position, glm::vec4 colour, float scale)
{
	cAABB pTestAABB;

	pTestAABB.boxWidth = scale;
	pTestAABB.minXYZ.x = floor(position.x / pTestAABB.boxWidth);
	pTestAABB.minXYZ.y = floor(position.y / pTestAABB.boxWidth);
	pTestAABB.minXYZ.z = floor(position.z / pTestAABB.boxWidth);
	pTestAABB.UpdateMaxFromMinAndWidth();

	//keep last drawn id 
	this->aabbID = pTestAABB.CalculateID(pTestAABB.minXYZ);

	// And even more drawing...
	iGameObject* aabbObject = this->aabb;

	RenderInfo aabbRender;
	//set values
	aabbRender.position.x = pTestAABB.minXYZ.x * pTestAABB.boxWidth;
	aabbRender.position.y = pTestAABB.minXYZ.y * pTestAABB.boxWidth;
	aabbRender.position.z = pTestAABB.minXYZ.z * pTestAABB.boxWidth;

	aabbRender.isWireframe = true;
	aabbRender.isDrawn = true;

	aabbRender.color = glm::vec4(colour.r, colour.g, colour.b, 1.0f);
	aabbRender.useDebugColorsNoLighting = true;

	aabbRender.scale = scale;

	aabbObject->SetRender(aabbRender);
	//draw
	this->DrawGameObject(aabbObject);
	
	return;
}
