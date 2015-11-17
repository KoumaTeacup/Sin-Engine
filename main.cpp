//main.cpp
#include "SESin.h"

#include "SEScene.h"
#include "SEGameObject.h"
#include "SEComController.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"


int main()
{
	// Global configuration.
	if (!SIN.init()) return -1;
	SIN.setFPSLimit(60);

	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------
	SEScene scene1;
	{
		SEGameObject	*gameObj = NULL; 
		SEComRenderer	*comRend = NULL;
		SEComTransform	*comTran = NULL;
		SEComCamera		*comCam = NULL;
		SEComController *comCtrl = NULL;

		// Allocate Game Objs
		gameObj = new SEGameObject("test");	// Visiable objs
		{
			// Allocate Components
			comRend = new SEComRenderer;
			comTran = new SEComTransform;
			comCtrl = new SEComController;

			// Configure Components

			// Attach External File
			comRend->attach("turret.obj");

			(*comTran)[rx] = 30;
			(*comTran)[ry] = 60;
			(*comTran)[sx] = 1.3f;
			(*comTran)[sz] = 0.7f;

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(comTran);
			//gameObj->attach(COM_RIGIDBODY);

			// Load Game Object
			scene1.InstantiateOnce(gameObj);

			(*comTran)[sx] += 1.0f;
			(*comTran)[sz] += 1.0f;
			(*comTran)[rx] = 25.0f;
			(*comTran)[ry] = 5.0f;
			(*comTran)[ty] -= 5.0f;
			gameObj->attach(comTran);
			gameObj->detach(COM_RIGIDBODY);

			gameObj->attach(comCtrl);
			scene1.InstantiateOnce(gameObj);
			//const unsigned TEST = scene1.load(gameObj);
			//for (int i = 0; i < 200; i++)
			//	scene1.instantiate(TEST);
		}

		gameObj = new SEGameObject("mainCamera"); // Camera obj
		{
			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			comTran = new SEComTransform;
			(*comTran)[tx] = 3.0f;
			(*comTran)[tz] = 12.0f;
			(*comTran)[rx] = -10.0f;
			(*comTran)[ry] = -10.0f;
			gameObj->attach(comTran);

			scene1.InstantiateOnce(gameObj);
		}

		delete gameObj;
		delete comRend;
		delete comTran;
		delete comCam;
		delete comCtrl;
	}
	// Start Scene Loop
	SIN.beginScene(scene1);

	// Release Scene Resources
	SIN_Release;
	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------

	return 0;
}