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
			(*comTran)[sx] = 0.6f;
			(*comTran)[sz] = 0.4f;

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