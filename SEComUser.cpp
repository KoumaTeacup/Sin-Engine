#include "SEComUser.h"

SEComUser::SEComUser(std::string name, std::string tag, SEGameObject * owner):
	SEComponent(COM_USER, name, tag, owner) {}

SEComUser::SEComUser(const SEComUser & rhs) :
	SEComponent::SEComponent(rhs){}

SEComUser & SEComUser::operator=(const SEComUser & rhs) {
	SEComponent::operator=(rhs);
	return *this;
}
