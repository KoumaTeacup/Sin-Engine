#include "windUpdater.h"
#include "SESin.h"
#include "SEEvent.h"

void WindUpdater::onUpdate() {
	SEEvent event("windDir");
	
	event.info.collisionDirction = globalWind;

	SIN.broadcast(event);
}
