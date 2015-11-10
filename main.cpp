//main.cpp
#include "SESin.h"

#include "SELog.h"
#include "SEUtility.h"
#include "SEGameObject.h"


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

		// Allocate Game Objs
		// Internal Deletion, Game Objs must be allocated dynamically!
		gameObj = new SEGameObject("test");	// Visiable objs
		{
			// Allocate Components
			// Internal Deletion, Components must be allocated dynamically!
			SEComRenderer	*comRend = new SEComRenderer(gameObj);
			SEComTransform	*comTran = new SEComTransform(gameObj);

			// Configure Components

			// Attach External File
			comRend->attach("test.vao");

			(*comTran)[rx] = 30;
			(*comTran)[ry] = 60;
			(*comTran)[sx] = 1.3f;
			(*comTran)[sz] = 0.7f;

			// Attach Components
			gameObj->attach(comRend);
			gameObj->attach(comTran);

			// Load Game Object
			scene1.load(gameObj);
		}

		gameObj = new SEGameObject("mainCamera"); // Camera obj
		{
			// Attach Default Components
			gameObj->attach(COM_CAMERA);
			comTran = new SEComTransform(gameObj);
			(*comTran)[tx] = 3.0f;
			(*comTran)[tz] = 12.0f;
			(*comTran)[rx] = -10.0f;
			(*comTran)[ry] = -10.0f;
			gameObj->attach(comTran);

			scene1.load(gameObj);
		}
	}

	// Start Scene Loop
	SIN.begin(scene1);

	// Release Scene Resources
	SIN_Release;
	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------

	return 0;
}