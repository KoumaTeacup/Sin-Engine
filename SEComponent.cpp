#include "SEComponent.h"

SEComponent::SEComponent(componentType t, SEGameObject* o): type(t), owner(o) {
}

SEComponent::SEComponent(const SEComponent& rhs): type(rhs.type), owner(rhs.owner) {
}
