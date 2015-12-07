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
		Hull *comHull = NULL;
		Turret *comTurr = NULL;

		// Allocate Game Objs
		gameObj = new SEGameObject("room", "room");	// Visiable objs
		{
			// Allocate Components
			comRend = new SEComRenderer;
			comTran = new SEComTransform;
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
				));/*
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(1.0f, 0.0f, 0.0f)),
				SEVector3f(-366.0f, 0.0f, 0.0f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(-1.0f, 0.0f, 0.0f)),
				SEVector3f(366.0f, 0.0f, 0.0f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(-0.68f, 0.0f, -0.73f)),
				SEVector3f(88.4f, 0.0f, 94.9f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(0.68f, 0.0f, -0.73f)),
				SEVector3f(-88.4f, 0.0f, 94.9f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(0.68f, 0.0f, 0.73f)),
				SEVector3f(-88.4f, 0.0f, -94.9f)
				));
			comColl->set(SECollider(
				SECollider::plane,
				SECollider::colliderShape(SEVector3f(-0.68f, 0.0f, 0.73f)),
				SEVector3f(88.4f, 0.0f, -94.9f)
				));*/

			// Attach External File
			comRend->attach("room.obj");
			comRend->attach("DM_Room.jpg");
			comRend->TextureRepeat(1.0f);

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(comTran);
			gameObj->attach(comColl);

			// Load Game Object
			scene1.InstantiateOnce(gameObj);
			delete comColl;
			delete comRend;
			delete comTran;
			delete gameObj;
		}

		gameObj = new SEGameObject("hull"); // hull obj
		{
			comTran = new SEComTransform;
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
			
			gameObj->attach(comTran);
			gameObj->attach(comRend);
			gameObj->attach(COM_RIGIDBODY);
			gameObj->attach(comColl);
			gameObj->attach(comHull);

			pHull = scene1.InstantiateOnce(gameObj);

			delete comTran;
			delete comRend;
			delete comCtrl;
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
				SEVector3f(0.0f, 14.0f, -14.0f)));

			comRend->attach("bullet.obj");
			comRend->attach("DM_bullet.jpg");

			gameObj->attach(COM_TRANSFORM);
			gameObj->attach(comRend);
			//gameObj->attach(comColl);
			bulletComp = gameObj->attach(&Bullet());

			bulletObj = scene1.load(gameObj);

			delete comRend;
			delete comColl;
			delete gameObj;
		}

		gameObj = new SEGameObject("turret"); // hull obj
		{
			comTran = new SEComTransform;
			comRend = new SEComRenderer(1.0f);
			comCtrl = new SEComController;
			comCons = new SEComConstraint;

			comCons->set(constraint(CONSTRAINT_PARENT, scene1.getInst(pHull)));

			comRend->attach("turret.obj");
			comRend->attach("DM_turret.png");

			gameObj->attach(comTran);
			gameObj->attach(comRend);
			gameObj->attach(comCtrl);
			gameObj->attach(comCons);
			gameObj->attach(&Turret(bulletObj, bulletComp));

			pTurret = scene1.InstantiateOnce(gameObj);

			delete comTran;
			delete comRend;
			delete comCtrl;
			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("gun"); // hull obj
		{
			comTran = new SEComTransform;
			comRend = new SEComRenderer(10.0f);
			comCtrl = new SEComController;
			comCons = new SEComConstraint;

			comCons->set(constraint(CONSTRAINT_PARENT, scene1.getInst(pTurret)));

			comRend->attach("gun.obj");
			comRend->attach("DM_turret.png");

			gameObj->attach(comTran);
			gameObj->attach(comRend);
			gameObj->attach(comCtrl);
			gameObj->attach(comCons);

			scene1.InstantiateOnce(gameObj);

			delete comTran;
			delete comRend;
			delete comCtrl;
			delete comCons;
			delete gameObj;
		}

		gameObj = new SEGameObject("thirdPersonCamera"); // Camera obj
		{
			comCons = new SEComConstraint;
			comTran = new SEComTransform(
				SEVector3f(0.0f, 150.0f, 200.0f),
				SEVector3f(-30.0f, 0.0f, 0.0f),
				SEVector3f(1.0f, 1.0f, 1.0f)
				);

			comCons->set(constraint(CONSTRAINT_POINT, scene1.getInst(pHull)));
			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			gameObj->attach(comTran);
			gameObj->attach(comCons);

			scene1.InstantiateOnce(gameObj);

			delete comCons;
			delete comTran;
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
				SECollider::colliderShape(SEVector3f(10.0f, 12.0f, 10.0f)),
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

		gameObj = new SEGameObject("enemyFactory"); // enemy obj
		{
			gameObj->attach(&EnemyFactory(ufo, 0));

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