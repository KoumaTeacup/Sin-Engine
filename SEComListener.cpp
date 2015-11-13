#include "SEComListener.h"

#include "SEEvent.h"



SEComListener::SEComListener(SEGameObject * owner, std::string name, std::string tag) :
	SEComponent(COM_LISTENER, owner, name, tag){
}

SEComListener::SEComListener(const SEComListener & rhs):
	SEComponent(rhs){
}

SEComListener & SEComListener::operator=(const SEComListener & rhs){
	SEComponent::operator=(rhs);
	return *this;
}