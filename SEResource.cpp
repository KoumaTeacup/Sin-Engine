#include "SEResource.h"

#include "SELog.h"
#include "shader.h"
#include "vao.h"

using namespace se_data;

SEResource* SEResource::resourceManager = nullptr;

SEResource& SEResource::getObj() {
	if (!resourceManager) resourceManager = new SEResource();
	return *resourceManager;
}

void SEResource::release() {
	if (resourceManager)
		delete resourceManager;
}

SEFilePointer SEResource::load(const char* filename) {
	// Determine file type.
	std::string name(std::string(filename).substr(0, std::string(filename).find('.')));
	const char* ext = strrchr(filename, '.') + 1;

	// Check Existance
	if (userResources.find(name) == userResources.end()) {
		if (strcmp(ext, "vert") == 0 || strcmp(ext, "frag") == 0)
			userResources[name] = new SEShader(name, RESTYPE_SHADER);
		else if (strcmp(ext, "vao") == 0)
			userResources[name] = new SEVAO(name, RESTYPE_VERTEX_ARRAY);
		//else if (strcmp(ext, "bmp") == 0)
			//userResources[name] = new SETexture(name, RESTYPE_TEXTURE);
#ifdef SE_DEBUG
		else {
			char log[64];
			sprintf(log, "File \"%s\" of type \".%s\" is not supported.", filename, ext);
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
			return SEFilePointer(NULL);
		}
#endif
	}

	// load file
	if (userResources[name]->load(filename))
		return SEFilePointer(userResources[name]);

#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Failed to create resource from file \"%s\".", filename);
	SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
	return SEFilePointer(NULL);
}

void SEResource::erase(std::string name) {
#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Internal file %s is released.", name.c_str());
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	userResources.erase(name);
}