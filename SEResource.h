#ifndef SERESOURCE_H
#define SERESOURCE_H

#include <unordered_map>

#define SE_Resource SEResource::getObj()

enum resourceType {
	RESTYPE_VERTEX_SHADER,
	RESTYPE_FRAGMENT_SHADER,
	RESTYPE_VERTEX_ARRAY,
	RESTYPE_IMAGE_DIFFUSEMAP
};

class SEFile {
public:
	SEFile(resourceType);
	virtual ~SEFile() {};

	virtual void load();
	virtual void unload();

private:
	int refCount;
	const char* name;
	resourceType type;
};

class SEResource {
public:

	static SEResource& getObj();
	static void release();

	SEFile* load(const char* filename);
	void	unload(const char* filename);

private:
	SEResource() {};

	static SEResource* resourceManager;

	std::unordered_map<const char*, SEFile*> userResources;
};

#endif