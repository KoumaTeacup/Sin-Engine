//main.cpp
#include "SESin.h"

#include "SELog.h"
#include "SEVector.h"
#include "SEMatrix.h"
#include "SEUtility.h"
#include "shader.h"
#include "SEGameObject.h"


int main()
{
	if (!SIN.init()) return -1;

	//------------------------------------------------------------

	SEGameObject gameObj;

	gameObj[COM_RENDERER].attach("default.vert");
	gameObj[COM_RENDERER].attach("default.frag");
	gameObj[COM_RENDERER].attach("test.vao");
	//------------------------------------------------------------

	SEMatrix4f PVM =
		SE_MATRIX_PROJECT4(1000.0f, 0.1f, 0.4f, 0.3f)*
		SE_MATRIX_TRANSLATE4(1.0f, 1.0f, -10.0f)*
		SE_MATRIX_SCALE4(0.5f, 0.5f, 1.0f)*
		SE_MATRIX_ROTATE4(se_data::AXIS_X, 50)
		//SE_MATRIX_ROTATE4(se_data::AXIS_Y, 30)
		;
	testShader.setVal(UNIFORM_MATRIX, "PVM", &PVM);

	SE_Utility.setFPSLimit(60);

	//------------------------------------------------------------
	SEScene scene1;
	scene1.load();

	SIN.begin(scene1);

	SIN_Release;

	return 0;
}