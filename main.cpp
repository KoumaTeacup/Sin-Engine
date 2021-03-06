//main.cpp
#include "SESin.h"

#include "SEScene.h"
#include "SEGameObject.h"
#include "SEComController.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"
#include "SEComCollider.h"
#include "SEComConstraint.h"
#include "hull.h"
#include "turret.h"
#include "enemy.h"
#include "ufo.h"
#include "bullet.h"
#include "bot.h"
#include "menu.h"


int main()
{
	// Global configuration.
	if (!SIN.init()) return -1;
	SIN.setFPSLimit(60);
	SIN.backFaceCulling();

	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------
	SEScene scene1;
	{
		unsigned pPlane;
		unsigned pCam;
		unsigned pHull;
		unsigned pTurret;
		unsigned ufo;
		unsigned bot;
		unsigned bulletObj;
		unsigned bulletComp;

		SEGameObject	*gameObj = NULL; 
		SEComRenderer	*comRend = NULL;
		SEComTransform	*comTran = NULL;
		SEComCamera		*comCam = NULL;
		SEComController *comCtrl = NULL;
		SEComCollider	*comColl = NULL;
		SEComConstraint *comCons = NULL;
		Hull			*comHull = NULL;
		Turret			*comTurr = NULL;

		// Allocate Game Objs
		gameObj = new SEGameObject("room", "room");	// Visiable objs
		{
			// Allocate Components
			comRend = new SEComRenderer;
			comColl = new SEComCollider;

			// Configure Components
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(0.0f, 1.0f, 0.0f))
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(0.0f, 0.0f, -1.0f)),
				SEVector3f(0.0f, 0.0f, -233.0f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(0.0f, 0.0f, 1.0f)),
				SEVector3f(0.0f, 0.0f, 233.0f)
				));

			// Attach External File
			comRend->attach("room.obj");
			comRend->attach("DM_Room.jpg");
			comRend->TextureRepeat(1.0f);

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comColl);

			// Load Game Object
			scene1.InstantiateOnce(gameObj);

			delete comColl;
			delete comRend;
			delete gameObj;
		}

		gameObj = new SEGameObject("hull"); // hull obj
		{
			comRend = new SEComRenderer;
			comColl = new SEComCollider;
			comHull = new Hull;

			// Configure Copmonents
			comColl->set(SECollider(
				SECollider::AABB,
				SECollider::colliderShape(SEVector3f(15.0f, 20.0f, 16.0f)),
				SEVector3f(0.0f, 10.0f, 0.0f)
				));

			comRend->attach("hull.obj");
			comRend->attach("DM_turret.png");
			
			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(COM_RIGIDBODY);
			gameObj->attach(comColl);
			gameObj->attach(comHull);

			pHull = scene1.InstantiateOnce(gameObj);

			delete comRend;
			delete comColl;
			delete comHull;
			delete gameObj;
		}

		gameObj = new SEGameObject("bullet", "bullet"); // hull obj
		{
			comRend = new SEComRenderer;
			comColl = new SEComCollider;

			// Configure Copmonents
			comColl->set(SECollider(
				SECollider::point,
				0.0f,
				SEVector3f(0.0f, 12.0f, -14.0f)));

			comRend->attach("bullet.obj");
			comRend->attach("DM_bullet.jpg");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(comColl);
			bulletComp = gameObj->attach(&Bullet());

			bulletObj = scene1.load(gameObj);

			delete comRend;
			delete comColl;
			delete gameObj;
		}

		gameObj = new SEGameObject("turret"); // hull obj
		{
			comRend = new SEComRenderer(1.0f);
			comCons = new SEComConstraint;

			comCons->set(constraint(CONSTRAINT_PARENT, scene1.getInst(pHull)));

			comRend->attach("turret.obj");
			comRend->attach("DM_turret.png");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(comCons);
			gameObj->attach(&Turret(bulletObj, bulletComp));

			pTurret = scene1.InstantiateOnce(gameObj);

			delete comRend;
			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("gun"); // hull obj
		{
			comRend = new SEComRenderer(10.0f);
			comCons = new SEComConstraint;

			comCons->set(constraint(CONSTRAINT_PARENT, scene1.getInst(pTurret)));

			comRend->attach("gun.obj");
			comRend->attach("DM_turret.png");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(comCons);

			scene1.InstantiateOnce(gameObj);

			delete comRend;
			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("thirdPersonCamera"); // Camera obj
		{
			comCons = new SEComConstraint;

			comCons->set(constraint(CONSTRAINT_POINT, scene1.getInst(pHull)));

			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			gameObj->attach(new SEComTransform(
				SEVector3f(0.0f, 150.0f, 200.0f),
				SEVector3f(-30.0f, 0.0f, 0.0f),
				SEVector3f(1.0f, 1.0f, 1.0f)
				));
			gameObj->attach(comCons);

			pCam = scene1.InstantiateOnce(gameObj);

			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("plane"); // Camera obj
		{
			comCons = new SEComConstraint;
			comRend = new SEComRenderer;

			comCons->set(constraint(CONSTRAINT_PARENT, scene1.getInst(pCam)));

			comRend->attach("plane.obj");
			comRend->attach("plane.jpg");
			comRend->attach("menu.vert");
			comRend->attach("menu.frag");

			// Attach Default Components
			gameObj->attach(comCons);
			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);

			gameObj->disable();

			pPlane = scene1.InstantiateOnce(gameObj);

			delete comRend;
			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("roomLight"); // Light obj
		{
			gameObj->attach(COM_LIGHT);
			comTran = new SEComTransform(
				SEVector3f(0.0f, 40.0f, 0.0f),
				SEVector3f(0.0f, 0.0f, 0.0f),
				SEVector3f(1.0f, 1.0f, 1.0f)
				);
			gameObj->attach(comTran);

			int pointLightId = scene1.load(gameObj);

			delete comTran;
			delete gameObj;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tz] = -200.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tz] = 200.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = 120.0f;
			(*comTran)[tz] = -110.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = -120.0f;
			(*comTran)[tz] = -110.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = 120.0f;
			(*comTran)[tz] = 110.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = -120.0f;
			(*comTran)[tz] = 110.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = -270.0f;

			gameObj = scene1.getInst(scene1.instantiate(pointLightId));
			comTran = static_cast<SEComTransform*>((*gameObj)[COM_TRANSFORM]);
			(*comTran)[tx] = 270.0f;
		}

		gameObj = new SEGameObject("ufoObject", "ufo"); // ufo obj
		{
			comRend = new SEComRenderer;
			comColl = new SEComCollider;

			// Configure Copmonents
			comColl->set(SECollider(
				SECollider::AABB,
				SECollider::colliderShape(SEVector3f(12.0f, 12.0f, 12.0f)),
				SEVector3f(0.0f, 0.0f, 0.0f)
				));

			comRend->attach("UFO.obj");
			comRend->attach("DM_UFO.png");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(COM_RIGIDBODY);
			gameObj->attach(comColl);
			gameObj->attach(&UFO());

			ufo = scene1.load(gameObj);

			delete comRend;
			delete comColl;
			delete gameObj;
		}

		gameObj = new SEGameObject("botObject", "bot"); // bot obj
		{
			comRend = new SEComRenderer;
			comColl = new SEComCollider;

			// Configure Copmonents
			comColl->set(SECollider(
				SECollider::AABB,
				SECollider::colliderShape(SEVector3f(10.0f, 12.5f, 10.0f)),
				SEVector3f(0.0f, 6.25f, 0.0f)
				));

			comRend->attach("bot.obj");
			comRend->attach("DM_UFO.png");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			gameObj->attach(COM_RIGIDBODY);
			gameObj->attach(comColl);
			gameObj->attach(&Bot());

			bot = scene1.load(gameObj);

			delete comRend;
			delete comColl;
			delete gameObj;
		}

		gameObj = new SEGameObject("enemyFactory"); // enemy obj
		{
			gameObj->attach(&EnemyFactory(ufo, bot));
			gameObj->attach(&Menu(pPlane));

			scene1.InstantiateOnce(gameObj);

			delete gameObj;
		}
	}
	// Start Scene Loop
	SIN.beginScene(scene1);

	// Release Scene Resources
	SIN_Release;
	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------

	return 0;
}