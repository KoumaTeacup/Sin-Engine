//main.cpp
#include "SESin.h"

#include "SEScene.h"
#include "SEGameObject.h"
#include "SEComController.h"
#include "SEComRenderer.h"
#include "SEComTransform.h"
#include "SEComCamera.h"
#include "cloth.h"
#include "freeCamera.h"
#include "windUpdater.h"
#include "spring.h"

int main()
{
	// Global configuration.
	if (!SIN.init()) return -1;
	SIN.setFPSLimit(60);

	glDisable(GL_CULL_FACE);

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
		FreeCameraController *comCamController = NULL;
		WindUpdater		*comWindUpdater = NULL;
		Spring			*comSpring = NULL;

		unsigned flagObject;
		// Allocate Game Objs
		gameObj = new SEGameObject("middleFlag");	// Visiable objs
		{
			// Allocate Components
			comRend = new SEComRenderer();
			comTran = new SEComTransform();
			comCtrl = new SEComController();
			comCloth = new Cloth(1);
			comSpring = new Spring(0.0f);

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
			gameObj->attach(comCloth);
			gameObj->attach(comCtrl);
			gameObj->attach(comSpring);

			// Load Game Object
			flagObject = scene1.load(gameObj);

			delete gameObj;
			delete comRend;
			delete comTran;
			delete comCtrl;
			delete comCloth;
		}

		SEGameObject *pFlag;
		SEComTransform *flagTrans;
		pFlag = scene1.instantiate(flagObject);
		flagTrans = static_cast<SEComTransform *>((*pFlag)[COM_TRANSFORM]);
		(*flagTrans)[tz] = 10.0f;

		pFlag = scene1.instantiate(flagObject);
		flagTrans = static_cast<SEComTransform *>((*pFlag)[COM_TRANSFORM]);
		(*flagTrans)[tz] = -10.0f;
		(*flagTrans)[ry] = 180.0f;

		pFlag = scene1.instantiate(flagObject);
		flagTrans = static_cast<SEComTransform *>((*pFlag)[COM_TRANSFORM]);
		(*flagTrans)[tx] = 10.0f;
		(*flagTrans)[ry] = 90.0f;

		pFlag = scene1.instantiate(flagObject);
		flagTrans = static_cast<SEComTransform *>((*pFlag)[COM_TRANSFORM]);
		(*flagTrans)[tx] = -10.0f;
		(*flagTrans)[ry] = -90.0f;

		gameObj = new SEGameObject("mainCamera"); // Camera obj
		{
			comCam = new SEComCamera();
			comCamController = new FreeCameraController();
			// Attach Default Components
			comCam->setMode(CAMERA_FOCUS_MODE);
			comCam->setFocus(SEVector3f(0.0f, 0.0f, 0.0f));
			comCam->setUp(SEVector3f(0.0f, 1.0f, 0.0f));
			gameObj->attach(comCam);
			comTran = new SEComTransform;
			(*comTran)[tx] = 30.0f;
			gameObj->attach(comTran);
			gameObj->attach(comCamController);

			scene1.InstantiateOnce(gameObj);

			delete gameObj;
			delete comCam;
			delete comCamController;
		}

		gameObj = new SEGameObject("windUpdater");
		{
			comWindUpdater = new WindUpdater();
			gameObj->attach(comWindUpdater);

			scene1.InstantiateOnce(gameObj);

			delete gameObj;
			delete comWindUpdater;
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