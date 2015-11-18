#include "SEResource.h"

#include "SELog.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"

using namespace se_system;

se_data::SEFilePointer SEResource::load(const char* filename) {
	// Determine file type.
	std::string name(std::string(filename).substr(0, std::string(filename).find('.')));
	const char* ext = strrchr(filename, '.') + 1;
	std::string resourceName(filename);

	if (strcmp(ext, "vert") == 0 || strcmp(ext, "frag") == 0)
		resourceName = name + ".shader";

	// Check Existance
	if (userResources.find(resourceName) == userResources.end()) {
		if (strcmp(ext, "vert") == 0 || strcmp(ext, "frag") == 0)
			userResources[resourceName] = new se_data::SEShader(resourceName);
		else if (strcmp(ext, "obj") == 0)
			userResources[resourceName] = new se_data::SEVAO(resourceName);
		else if (strcmp(ext, "png") == 0 || strcmp(ext, "bmp") == 0 ||
			strcmp(ext, "jpg") == 0 || strcmp(ext, "gif") == 0)
			userResources[resourceName] = new se_data::SETexture(resourceName, 1);
#ifdef SE_DEBUG
		else {
			char log[64];
			sprintf(log, "File \"%s\" of type \".%s\" is not supported.", filename, ext);
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
			return se_data::SEFilePointer(NULL);
		}
#endif
	}

	// load file
	if (userResources[resourceName]->load(filename))
		return se_data::SEFilePointer(userResources[resourceName]);

#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Failed to create resource from file \"%s\".", filename);
	SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
	return se_data::SEFilePointer(NULL);
}

void SEResource::erase(std::string name) {
#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Internal file %s is released.", name.c_str());
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	userResources.erase(name);
}