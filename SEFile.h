#ifndef SEFILE_H
#define SEFILE_H

#include "SEObject.h"

namespace se_data {

enum resourceType {
	RESTYPE_UNDEFINED,
	RESTYPE_SHADER,
	RESTYPE_VERTEX_ARRAY,
	RESTYPE_TEXTURE
};

// SEFile class is not allowed for any copy operations.
class SEFile : public se_system::SEObject{
	friend class SEFilePointer;

public:
	// Must have name and type
	SEFile(std::string n, resourceType t, std::string tag = std::string());
	virtual ~SEFile();

	// Inherited class should override file loading process.
	virtual bool load(const char* filename) = 0;

	// Setters & Getters
	SEFile*		 getFile()		 { return this; }
	resourceType getType() const { return type; }

	virtual const char* toString() const;

private:
	SEFile& operator=(const SEFile &rhs) {}

	int refCount;
	resourceType type;
};

class SEFilePointer {
public:
	SEFilePointer();
	SEFilePointer(SEFile* pF);
	SEFilePointer(const SEFilePointer& rhs);

	~SEFilePointer();

	SEFilePointer&	operator=(const SEFilePointer& rhs);
	SEFile*			operator->() const;

private:
	SEFile *pFile;
};

}

typedef se_data::SEFilePointer SE_File;

#endif