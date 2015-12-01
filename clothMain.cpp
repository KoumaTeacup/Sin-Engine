//main.cpp
#include "SESin.h"

#include "SEScene.h"
#include "SEGameObject.h"
#include "SEComController.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"
#include "cloth.h"


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
		Cloth			*comCloth = NULL;

		// Allocate Game Objs
		gameObj = new SEGameObject("middleFlag");	// Visiable objs
		{
			// Allocate Components
			comRend = new SEComRenderer();
			comTran = new SEComTransform();
			comCtrl = new SEComController();
			comCloth = new Cloth(1);

			// Configure Components
			(*comTran)[ty] = 7.0f;

			// Attach External File
			comRend->attach("cloth_12x12.obj");
			comRend->attach("Digipen_flag.jpg");
			comRend->attach("cloth.vert");
			comRend->attach("cloth.frag");

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(comTran);
			gameObj->attach(comCtrl);
			gameObj->attach(comCloth);
			//gameObj->attach(COM_RIGIDBODY);

			// Load Game Object
			scene1.InstantiateOnce(gameObj);
		}

		gameObj = new SEGameObject("mainCamera"); // Camera obj
		{
			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			delete comTran;
			comTran = new SEComTransform;
			(*comTran)[tz] = 30.0f;
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