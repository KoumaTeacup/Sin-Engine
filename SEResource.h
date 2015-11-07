#ifndef SERESOURCE_H
#define SERESOURCE_H

#include <unordered_map>

#define SE_Resource			SEResource::getObj()
#define SE_Resource_Release	SEResource::release()

enum resourceType {
	RESTYPE_UNDEFINED,
	RESTYPE_SHADER,
	RESTYPE_VERTEX_ARRAY,
	RESTYPE_TEXTURE
};

class SEFile {
public:
	SEFile(const char* n, resourceType t);
	virtual ~SEFile();

	virtual bool load(const char* filename);
	virtual int	 unload();

	resourceType getType() { return type; }

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
	void	unload(SEFile* res);

private:
	SEResource() {};

	static SEResource* resourceManager;

	std::unordered_map<const char*, SEFile*> userResources;
};

#endif