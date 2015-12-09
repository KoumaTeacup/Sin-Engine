#include "menu.h"

#include "SEEvent.h"
#include "SEGameObject.h"
#include "SESin.h"
#include "SEScene.h"

void Menu::handle(SEEvent & e)
{
	if (strcmp(e.infoString, "win") == 0)
		SIN.getActiveScene()->getInst(planeObj)->enable();
}
