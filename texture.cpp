#include "texture.h"

#include "SESin.h"
#include "SELog.h"

using namespace se_data;

SETexture::SETexture(std::string name, unsigned u) :
	SEFile(name, RESTYPE_TEXTURE),
	id(0),
	unit(u),
	target(GL_TEXTURE_2D) {}

SETexture::~SETexture() {

}

bool SETexture::load(const char * filename) {
#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Loading texture file %s...", filename);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	if (!texture.loadFromFile(filename)) {
#ifdef SE_DEBUG
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "failed.");
		sprintf(log, "Failed to load image file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
		return false;
	}

	glGenTextures(1, &id);
	glBindTexture(target, id);
	glTexImage2D(target, 0, GL_RGBA,
		texture.getSize().x, texture.getSize().y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
	glGenerateMipmap(target);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(target, 0);

#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "success.");
#endif;

	return true;
}

void SETexture::onInit(){
}

void SETexture::onRelease(){
	glDeleteTextures(1, &id);
}

void SETexture::onDraw() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, id);
}

void SETexture::onPostUpdate() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, 0);
}