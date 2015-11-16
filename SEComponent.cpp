#include "SEComponent.h"

#include "SEGameObject.h"
#include "SELog.h"
#include "SEUtility.h"

int SEComponent::num = 0;

SEComponent::SEComponent(componentType t, std::string name, std::string tag, SEGameObject* o): 
	SEObject(name, tag, ++num),
	type(t), 
	owner(o) {}

SEComponent::SEComponent(const SEComponent& rhs): 
	SEObject(rhs),
	type(rhs.type), 
	owner(rhs.owner) {}

SEComponent& SEComponent::operator=(const SEComponent &rhs) {
#ifdef SE_DEBUG
	if (rhs.getType() != type) {
		char log[256];
		const char *name = toString();
		const char *rhsName = rhs.toString();
		sprintf(log,
			"Type Conflict: Copy assignment of %s to %s is not allowed. Operation canceled.",
			rhsName,
			name);
		free((void*)name); free((void*)rhsName);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
		return *this;
	}
#endif;
	SEObject::operator=(rhs);
	return *this;
}

const char* SEComponent::toString() const {
	char *output = (char*)malloc(sizeof(char) * 256);
	char str[32];
	const char* ownerName = getOwner().toString();
	SE_Utility.typeToString(str, type);
	sprintf(output, "%s > Componet \"%s\" of type %s ", ownerName, getName().c_str(), str);
	free((void*)ownerName);
	return output;
}