#include "SEComTransform.h"

#include "SEGameObject.h"
#include "SESin.h"
#include "SEComConstraint.h"

SEComTransform::SEComTransform(std::string name, std::string tag, SEGameObject* owner):
	SEComponent(COM_TRANSFORM, name, tag, owner),
	scale(1.0f,1.0f,1.0f){}

SEComTransform::SEComTransform(SEVector3f t, SEVector3f r, SEVector3f s, std::string name, std::string tag, SEGameObject * owner) :
	SEComponent(COM_TRANSFORM, name, tag, owner),
	trans(t), rot(r), scale(s) {}

SEComTransform::SEComTransform(const SEComTransform & rhs) :
	SEComponent::SEComponent(rhs),
	trans(rhs.trans),
	rot(rhs.rot),
	scale(rhs.scale){}

SEComTransform::~SEComTransform() {
}

SEComTransform& SEComTransform::operator=(const SEComTransform & rhs) {
	SEComponent::operator=(rhs);
	trans = rhs.trans;
	rot = rhs.rot;
	scale = rhs.scale;
	return *this;
}

void SEComTransform::onInit() {
	if (getOwner()[COM_CONSTRAINT]) {
		SE_CONSTRAINT.setLocalTransform(trans, rot, scale);
	}
}

void SEComTransform::onDraw() {
	SEComponent::onDraw();
	modelTr = translationMatrix()*rotationMatrix()*scaleMatrix();
}

float SEComTransform::operator[](index i) const {
	switch (i) {
	case tx: return trans[0];
	case ty: return trans[1];
	case tz: return trans[2];
	case rx: return rot[0];
	case ry: return rot[1];
	case rz: return rot[2];
	case sx: return scale[0];
	case sy: return scale[1];
	case sz: return scale[2];
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
	case tx: return trans[0];
	case ty: return trans[1];
	case tz: return trans[2];
	case rx: return rot[0];
	case ry: return rot[1];
	case rz: return rot[2];
	case sx: return scale[0];
	case sy: return scale[1];
	case sz: return scale[2];
	default:
#ifdef SE_DEBUG
		char log[256];
		const char* name = toString();
		sprintf(log, "%s> Invalid tramform component access, please use enum.", name);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
		free((void*)name);
#endif
		return SEVector3f()[0];
	}
}

SEMatrix4f SEComTransform::translationMatrix() const {
	return SE_MATRIX_TRANSLATE4(trans[0], trans[1], trans[2]);
}

SEMatrix4f SEComTransform::rotationMatrix() const {
	return 	SE_MATRIX_ROTATE4(se_data::AXIS_Y, rot[1]) *
		SE_MATRIX_ROTATE4(se_data::AXIS_X, rot[0]) *
		SE_MATRIX_ROTATE4(se_data::AXIS_Z, rot[2]);
}

SEMatrix4f SEComTransform::scaleMatrix() const {
	return SE_MATRIX_SCALE4(scale[0], scale[1], scale[2]);
}
