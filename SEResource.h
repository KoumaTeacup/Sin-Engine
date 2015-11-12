#ifndef SERESOURCE_H
#define SERESOURCE_H

#include <unordered_map>
#include <string>

#include "SESystem.h"

#define SE_Resource			se_system::SEResource::getObj()
#define SE_Resource_Release	se_system::SEResource::release()

namespace se_data {
	class SEFile;
	class SEFilePointer;
}

namespace se_system{


class SEResource : public SESystem<SEResource>{
	friend SESystem<SEResource>;
public:
	se_data::SEFilePointer load(const char *filename);
	void erase(std::string name);

private:
	SEResource() {};

	std::unordered_map<std::string, se_data::SEFile*> userResources;
};

}
#endif