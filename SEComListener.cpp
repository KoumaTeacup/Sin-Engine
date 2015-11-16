#include "SEComListener.h"

#include "SEEvent.h"

SEComListener::SEComListener(std::string name, std::string tag, SEGameObject * owner) :
	SEComponent(COM_LISTENER, name, tag, owner){
}

SEComListener::SEComListener(const SEComListener & rhs):
	SEComponent(rhs){
}

SEComListener & SEComListener::operator=(const SEComListener & rhs){
	SEComponent::operator=(rhs);
	return *this;
}