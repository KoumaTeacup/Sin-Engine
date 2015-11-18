#include "SEFile.h"

#include "SEResource.h"

using namespace se_data;

// class SEFile
SEFile::SEFile(std::string n, resourceType t, std::string tag) :
	SEObject(n, tag),
	type(t), 
	refCount(0){}

SEFile::~SEFile() {
}

const char* SEFile::toString() const {
	char *name = (char*)malloc(sizeof(char) * 32);
	sprintf(name, getName().c_str());
	return name;
}

// class SEFilePointer
SEFilePointer::SEFilePointer(): pFile(NULL){}

SEFilePointer::SEFilePointer(SEFile *pF) : pFile(pF) {
	if(pF) ++(pF->refCount);
}

SEFilePointer::SEFilePointer(const SEFilePointer& rhs) : pFile(rhs.pFile) {
	if (pFile) ++(pFile->refCount);
}

SEFilePointer::~SEFilePointer() {
	if (pFile) {
		if (pFile->refCount < 1) {
			SE_Resource.erase(pFile->getName());
			delete pFile;
		}
		else --(pFile->refCount);
	}
}

SEFilePointer& SEFilePointer::operator=(const SEFilePointer &rhs) {
	if (pFile) {
		if (pFile->refCount < 1) {
			SE_Resource.erase(pFile->getName());
			delete pFile;
		}
		else --(pFile->refCount);
	}
	pFile = rhs.pFile;
	if (pFile) ++(pFile->refCount);
	return *this;
}

SEFile* SEFilePointer::operator->() const {
	return pFile;
}