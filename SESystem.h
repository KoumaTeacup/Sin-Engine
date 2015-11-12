#ifndef SESYSTEM_H
#define SESYSTEM_H

namespace se_system {

template <typename T>
class SESystem {
public:
	virtual ~SESystem();
	static T& getObj();
	static void release();

protected:
	SESystem();
};

template <typename T>
SESystem<T>::SESystem() {}

template <typename T>
SESystem<T>::~SESystem() {}

template <typename T>
T& SESystem<T>::getObj() {
	static T *pInstance = nullptr;
	if (!pInstance) pInstance = new T;
	return *pInstance;
}

template <typename T>
void SESystem<T>::release() {
	delete &getObj();
}

}

#endif