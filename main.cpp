//main.cpp
#include "SESin.h"

#include "SEScene.h"
#include "SEGameObject.h"
#include "SEComController.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"


int _main()
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

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(comTran);
			//gameObj->attach(COM_RIGIDBODY);

			// Load Game Object
			scene1.InstantiateOnce(gameObj);
			//----------------------------------------------
			gameObj->attach(comTran);
			gameObj->detach(COM_RIGIDBODY);

			gameObj->attach(comCtrl);

			comRend->attach("DM_turret.png");
			gameObj->attach(comRend);

			scene1.InstantiateOnce(gameObj);
		}

		gameObj = new SEGameObject("mainCamera"); // Camera obj
		{
			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			delete comTran;
			comTran = new SEComTransform;
			(*comTran)[tz] = 20.0f;
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