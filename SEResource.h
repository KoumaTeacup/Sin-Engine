#ifndef SERESOURCE_H
#define SERESOURCE_H

#include <unordered_map>
#include <string>

#define SE_Resource			se_data::SEResource::getObj()
#define SE_Resource_Release	se_data::SEResource::release()



namespace se_data{

class SEFile;
class SEFilePointer;

class SEResource {
public:

	static SEResource& getObj();
	static void release();

	SEFilePointer load(const char *filename);
	void erase(std::string name);

private:
	SEResource() {};

	static SEResource* resourceManager;

	std::unordered_map<std::string, SEFile*> userResources;
};

}
#endif