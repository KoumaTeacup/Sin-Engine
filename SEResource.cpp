#include "SEResource.h"

#include "SELog.h"
#include "shader.h"
#include "vao.h"

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
			userResources[resourceName] = new se_data::SEShader(resourceName, se_data::RESTYPE_SHADER);
		else if (strcmp(ext, "vao") == 0)
			userResources[resourceName] = new se_data::SEVAO(resourceName, se_data::RESTYPE_VERTEX_ARRAY);
		//else if (strcmp(ext, "bmp") == 0)
			//userResources[name] = new SETexture(name, RESTYPE_TEXTURE);
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