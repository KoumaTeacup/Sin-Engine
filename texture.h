#ifndef SETEXTURE_H
#define SETEXTURE_H

#include <GL\glew.h>
#include <SFML\Graphics.hpp>

#include "SEFile.h"

namespace se_data {

class SETexture : public SEFile {
public:
	SETexture(std::string name, unsigned u = 0);
	~SETexture();

	bool load(const char* filename);

	const char* toString() const { return SEFile::toString(); }

protected:
	void onInit();
	void onRelease();
	void onDraw();
	void onPostUpdate();

private:
	GLuint id;
	GLenum target;
	unsigned unit;
	sf::Image texture;
	sf::Texture tex;
};

}

#endif