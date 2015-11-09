#include "SEComTransform.h"

SEComTransform::SEComTransform(SEGameObject* owner,	std::string name, std::string tag):
	SEComponent(COM_TRANSFORM, owner, name, tag) {
	data[scale][x] = data[scale][y] = data[scale][z] = 1.0f;
}

SEComTransform::SEComTransform(const SEComTransform & rhs) :
	SEComponent::SEComponent(rhs),
	data(rhs.data) {}

SEComTransform::~SEComTransform() {
}

SEComTransform& SEComTransform::operator=(const SEComTransform & rhs) {
	SEComponent::operator=(rhs);
	if(rhs.getType() == getType())
		data = rhs.data;
	return *this;
}

void SEComTransform::onDraw() {
	SEComponent::onDraw();
	modelTr =
		SE_MATRIX_SCALE4(data[scale][x], data[scale][y], data[scale][z]) *
		SE_MATRIX_ROTATE4(se_data::AXIS_Z, data[rotate][z]) *
		SE_MATRIX_ROTATE4(se_data::AXIS_X, data[rotate][x]) *
		SE_MATRIX_ROTATE4(se_data::AXIS_Y, data[rotate][y]) *
		SE_MATRIX_TRANSLATE4(data[trans][x], data[trans][y], data[trans][z]);
}

float SEComTransform::operator[](index i) const {
	switch (i) {
	case tx: return data[trans][x];
	case ty: return data[trans][y];
	case tz: return data[trans][z];
	case rx: return data[rotate][x];
	case ry: return data[rotate][y];
	case rz: return data[rotate][z];
	case sx: return data[scale][x];
	case sy: return data[scale][y];
	case sz: return data[scale][z];
	default: 
#ifdef SE_DEBUG
		char log[256];
		const char* name = toString();
		sprintf(log, "%s> Invalid tramform component access, please use enum.", name);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
		free((void*)name);
#endif
		return 0.0f;
	}
}

float& SEComTransform::operator[](index i) {
	switch (i) {
	case tx: return data[trans][x];
	case ty: return data[trans][y];
	case tz: return data[trans][z];
	case rx: return data[rotate][x];
	case ry: return data[rotate][y];
	case rz: return data[rotate][z];
	case sx: return data[scale][x];
	case sy: return data[scale][y];
	case sz: return data[scale][z];
	default:
#ifdef SE_DEBUG
		char log[256];
		const char* name = toString();
		sprintf(log, "%s> Invalid tramform component access, please use enum.", name);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
		free((void*)name);
#endif
		return data[0][0];
	}
}