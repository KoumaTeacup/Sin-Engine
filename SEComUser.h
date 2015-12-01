#ifndef USER_H
#define USER_H

#include "SEComponent.h"

class SEComUser : public SEComponent {
public:
	SEComUser(std::string name = std::string(),
		std::string tag = std::string(),
		SEGameObject *owner = NULL);
	SEComUser(const SEComUser &rhs);

	SEComUser& operator =(const SEComUser &rhs);

private:

};

#endif