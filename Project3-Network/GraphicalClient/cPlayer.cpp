#include "cPlayer.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

//Update
void cPlayer::Update(float deltaTime)
{

	//Player Controls
	bool pressW = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_W) == GLFW_PRESS;
	bool pressS = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_S) == GLFW_PRESS;

	bool pressA = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_A) == GLFW_PRESS;
	bool pressD = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_D) == GLFW_PRESS;

	bool pressE = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_E) == GLFW_PRESS;
	bool pressQ = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_Q) == GLFW_PRESS;

	//mOffset = glm::vec3(0.f, 0.5f, -1.f);
	if (pressE != pressQ)
	{
		float goUpward = pressE ? 18.0f : -18.0f;
		RenderInfo playerRender = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender();
		PhysicsInfo	playerPhysics = gelObjectManager::sharedInstance().GetMainPlayer()->GetPhysics();
		
		glm::vec4 unnormalizedPlayerDirection;// = glm::vec4(playerRender.transform[3].x, playerRender.transform[3].y, playerRender.transform[3].z, 0.0f);
		glm::vec4 forward(0.0f, goUpward, 0.0f, 0.0f);
		unnormalizedPlayerDirection = playerRender.transform * forward;

		//playerPhysics.velocity.x += unnormalizedPlayerDirection.x * deltaTime;
		//playerPhysics.velocity.z += unnormalizedPlayerDirection.z * deltaTime;

		playerRender.position.x += unnormalizedPlayerDirection.x * deltaTime * 0.8f;
		playerRender.position.y += unnormalizedPlayerDirection.y * deltaTime * 0.8f;
		playerRender.position.z += unnormalizedPlayerDirection.z * deltaTime * 0.8f;

		if (playerRender.position.y - 10.0f < 0.0f)
		{
			//playerRender.position.y = 10.0f;
		}

		this->renderInfo = playerRender;
		this->physicsInfo = playerPhysics;
	}

	if (pressW != pressS)
	{
		//try this

		//{
		//	rapidjson::Document initDoc;

		//	rapidjson::Value initData;
		//	initData.SetObject();

		//	rapidjson::Value duration(rapidjson::kNumberType);
		//	rapidjson::Value speed(rapidjson::kNumberType);

		//	duration.SetFloat(0.2f);
		//	speed.SetFloat(5.0f);
		//	float goForward = pressW ? 1.0 : -1.0f;

		//	rapidjson::Value direction(rapidjson::kObjectType);
		//	{
		//		rapidjson::Value destination_x(rapidjson::kNumberType);
		//		rapidjson::Value destination_y(rapidjson::kNumberType);
		//		rapidjson::Value destination_z(rapidjson::kNumberType);

		//		destination_x.SetFloat(0.0f);
		//		destination_y.SetFloat(0.0f);
		//		destination_z.SetFloat(goForward);

		//		// adding elements to contacts array.
		//		direction.AddMember("x", destination_x, initDoc.GetAllocator());
		//		direction.AddMember("y", destination_y, initDoc.GetAllocator());
		//		direction.AddMember("z", destination_z, initDoc.GetAllocator());
		//	}
		//	initData.AddMember("direction", direction, initDoc.GetAllocator());
		//	initData.AddMember("speed", speed, initDoc.GetAllocator());
		//	initData.AddMember("duration", duration, initDoc.GetAllocator());

		//	iCommand* p_comMoveB = new cApproachVelocityCommand();
		//	p_comMoveB->SetGameObject(this);
		//	p_comMoveB->Init(initData);
		//	//myCG.AddSerial( p_comMoveB );
		//	//cmdGrp.AddParallel(p_comMoveB);
		//	cmdGrp.AddParallelCommand(p_comMoveB);
		//}
		//GelCommandManager::sharedInstance().AddCommandGroup(this->cmdGrp);




		float goForward = pressW ? 1.6f : -1.6f;
		RenderInfo playerRender = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender();
		PhysicsInfo	playerPhysics = gelObjectManager::sharedInstance().GetMainPlayer()->GetPhysics();
		glm::mat4x4 playerTransform = glm::mat4x4(1.0f);

		//this will be abstracted
		playerTransform = glm::translate(playerTransform, glm::vec3(playerRender.position.x, playerRender.position.y, playerRender.position.z));
		glm::mat4 matRotation(playerRender.orientation);
		playerTransform = playerTransform * matRotation;
		//playerTransform = glm::rotate(playerTransform, playerRender.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		//playerTransform = glm::rotate(playerTransform, playerRender.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//playerTransform = glm::rotate(playerTransform, playerRender.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		playerTransform = glm::scale(playerTransform, glm::vec3(playerRender.lastDrawnActualScale, playerRender.lastDrawnActualScale, playerRender.lastDrawnActualScale));

		glm::vec4 unnormalizedPlayerDirection = glm::vec4(playerTransform[3].x, playerTransform[3].y, playerTransform[3].z, 0.0f);
		glm::vec4 forward(0.0f, 0.0f, goForward, 0.0f);
		glm::vec4 forwardPositive(0.0f, -1.0f, abs(goForward), 0.0f);
		unnormalizedPlayerDirection = playerTransform * forward;
		glm::vec4 unnormalizedPlayerDirection_positive = playerTransform * forwardPositive;
		glm::vec4 normalizedPlayerDirection = glm::normalize(unnormalizedPlayerDirection);

		this->lightInfo.direction = glm::normalize(unnormalizedPlayerDirection_positive);

		float playerSpeedMultiplier = GelsenizeCore::sharedInstance().GetPlayerSpeed();
		//playerPhysics.acceleration.x = normalizedPlayerDirection.x *playerSpeedMultiplier;
		//playerPhysics.acceleration.z = normalizedPlayerDirection.z *playerSpeedMultiplier;

		playerPhysics.velocity.x = normalizedPlayerDirection.x *playerSpeedMultiplier;
		playerPhysics.velocity.z = normalizedPlayerDirection.z *playerSpeedMultiplier;

		//playerRender.position.x += unnormalizedPlayerDirection.x * deltaTime * 0.8f;
		//playerRender.position.y += unnormalizedPlayerDirection.y * deltaTime * 0.8f;
		//playerRender.position.z += unnormalizedPlayerDirection.z * deltaTime * 0.8f;

		this->renderInfo = playerRender;
		this->physicsInfo = playerPhysics;
	}
	else //if (!(pressW || pressS))
	{
		//cCommandGroup cmdGrp;
		//{
		//	rapidjson::Document initDoc;

		//	rapidjson::Value initData;
		//	initData.SetObject();

		//	rapidjson::Value duration(rapidjson::kNumberType);
		//	rapidjson::Value speed(rapidjson::kNumberType);

		//	duration.SetFloat(0.4f);
		//	speed.SetFloat(0.0f);
		//	float goForward = pressW ? -1.0 : 1.0f;

		//	rapidjson::Value direction(rapidjson::kObjectType);
		//	{
		//		rapidjson::Value direction_x(rapidjson::kNumberType);
		//		rapidjson::Value direction_y(rapidjson::kNumberType);
		//		rapidjson::Value direction_z(rapidjson::kNumberType);

		//		direction_x.SetFloat(0.0f);
		//		direction_y.SetFloat(0.0f);
		//		direction_z.SetFloat(goForward);

		//		// adding elements to contacts array.
		//		direction.AddMember("x", direction_x, initDoc.GetAllocator());
		//		direction.AddMember("y", direction_y, initDoc.GetAllocator());
		//		direction.AddMember("z", direction_z, initDoc.GetAllocator());
		//	}
		//	initData.AddMember("direction", direction, initDoc.GetAllocator());
		//	initData.AddMember("speed", speed, initDoc.GetAllocator());
		//	initData.AddMember("duration", duration, initDoc.GetAllocator());

		//	iCommand* p_comMoveB = new cApproachVelocityCommand();
		//	p_comMoveB->SetGameObject(this);
		//	p_comMoveB->Init(initData);
		//	//myCG.AddSerial( p_comMoveB );
		//	//cmdGrp.AddParallel(p_comMoveB);
		//	cmdGrp.AddParallelCommand(p_comMoveB);
		//}
		//GelCommandManager::sharedInstance().AddCommandGroup(cmdGrp);


		this->physicsInfo.velocity.x -= this->physicsInfo.velocity.x * 0.9f * deltaTime;
		this->physicsInfo.velocity.z -= this->physicsInfo.velocity.z * 0.9f * deltaTime;


		/*PhysicsInfo	playerPhysics = gelObjectManager::sharedInstance().GetMainPlayer()->GetPhysics();
		playerPhysics.velocity.x -= playerPhysics.velocity.x * deltaTime;
		playerPhysics.velocity.z -= playerPhysics.velocity.z * deltaTime;

		if (playerPhysics.velocity.x < 0.0f)
			playerPhysics.velocity.x = 0.0f;
		if (playerPhysics.velocity.z < 0.0f)
			playerPhysics.velocity.z = 0.0f;
		gelObjectManager::sharedInstance().GetMainPlayer()->SetPhysics(playerPhysics);*/
	}

	if (pressA != pressD)
	{
		iGameObject *player = gelObjectManager::sharedInstance().GetMainPlayer();
		RenderInfo render = player->GetRender();
		float modifier_pressD = pressD ? -1.0f : +1.0f;
		//render.rotation.y = render.rotation.y + (modifier_pressD * 0.02f);

		//render.orientation.y += (modifier_pressD * 0.6f) *deltaTime;

		// Represents the change over 1 second.
		glm::quat qRotChangeZeroSeconds;		// No rotational change
		glm::quat qRotChange(glm::vec3(0.0f, (modifier_pressD * 0.6f), 0.0f));
		glm::quat qRotChangeThisFrame = glm::slerp(qRotChangeZeroSeconds, qRotChange, deltaTime);

		render.orientation *= qRotChangeThisFrame;

		player->SetRender(render);
	}
}

std::string cPlayer::ObjectType(void)
{
	return "player";
}

//Info setter-getters

LightInfo cPlayer::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cPlayer::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cPlayer::GetRender(void)
{
	return this->renderInfo;
}

PhysicsInfo cPlayer::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cPlayer::GetCollision(void)
{
	return this->collisionInfo;
}

void cPlayer::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cPlayer::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cPlayer::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cPlayer::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cPlayer::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

//get object id

int cPlayer::getObjectID()
{
	return this->objectID;
}

//constructer, destructer

cPlayer::cPlayer()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();
}

cPlayer::~cPlayer()
{
}
