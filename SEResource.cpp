#include <string>

#include "SEResource.h"

#include "SELog.h"
#include "shader.h"
#include "vao.h"

SEResource* SEResource::resourceManager = nullptr;

SEFile::SEFile(const char* n, resourceType t):name(n), type(t), refCount(0) {
}

SEFile::~SEFile() {
}

bool SEFile::load(const char* filename) {
	++refCount;
	return true;
}

int SEFile::unload() {
	return --refCount;
}

SEResource& SEResource::getObj() {
	if (!resourceManager) resourceManager = new SEResource();
	return *resourceManager;
}

void SEResource::release() {
	if (resourceManager)
		delete resourceManager;
}

SEFile* SEResource::load(const char* filename) {
	// Determine file type.
	resourceType type;
	const char* name = std::string(filename).substr(0, std::string(filename).find('.')).c_str();
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
			return NULL;
		}
#endif
	}

	// load file
	if (userResources[name]->load(filename)) return userResources[name];

#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Failed to create resource from file \"%s\".", filename);
	SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
	return NULL;
#endif
}

void SEResource::unload(const char* filename) {
	if (userResources.find(filename) != userResources.end()) {
		SEFile* file = userResources[filename];
		if (!file->unload()) {
			delete file;
			userResources.erase(filename);
		}
	}
}

void SEResource::unload(SEFile * res){
	if (!res) return;
	if (!res->unload()) {
		delete res;
	}
}
