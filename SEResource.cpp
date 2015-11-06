#include "SEResource.h"

SEResource* SEResource::resourceManager = nullptr;

SEResource& SEResource::getObj() {
	if (!resourceManager) resourceManager = new SEResource();
	return *resourceManager;
}

void SEResource::release() {
	if (resourceManager)
		delete resourceManager;
}