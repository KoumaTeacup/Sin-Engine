#include <fstream>

#include "vao.h"
#include "SELog.h"


SEVAO::SEVAO() {
}

bool SEVAO::load(const char* filename) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
#ifdef SE_DEBUG
		char log[64];
		sprintf(log, "Failed to open file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
		return false;
	}
	char buf[64];
	char sentry = ifs.peek();
	while (sentry != EOF) {
		ifs.getline(buf, 64);
		if (buf[0] != '#') continue;
		do {
			if (strcmp(buf, "#pnt")) {
				SEVector3f input3f;
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				pnt.push_back(input3f);

				sentry = ifs.peek();
			}

			if (strcmp(buf, "#nrm")) {
				SEVector3f input3f;
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				nrm.push_back(input3f);

				sentry = ifs.peek();
			}

			if (strcmp(buf, "#tex")) {
				SEVector2f input2f;
				ifs >> input2f[0] >> input2f[1];
				tex.push_back(input2f);

				sentry = ifs.peek();
			}

			if (strcmp(buf, "#tan")) {
				SEVector3f input3f;
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				tan.push_back(input3f);

				sentry = ifs.peek();
			}

			if (strcmp(buf, "#tri")) {
				SEVector3i input3i;
				ifs >> input3i[0] >> input3i[1] >> input3i[2];
				tri.push_back(input3i);

				sentry = ifs.peek();
			}

			if (strcmp(buf, "#quad")) {
				SEVector4i input4i;
				ifs >> input4i[0] >> input4i[1] >> input4i[2] >> input4i[3];
				quad.push_back(input4i);

				sentry = ifs.peek();
			}
		} while (sentry != EOF && sentry != '#');
	}
	ifs.close();
	return true;
}

bool SEVAO::initVAO(){
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);

	GLuint vbo;
	GLuint ibo;
	if (pnt.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * pnt.size(), &pnt[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (nrm.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nrm.size(), &nrm[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (tex.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * tex.size(), &tex[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (tan.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * tan.size(), &tan[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (tri.size() > 0) {
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * tri.size(), &tri[0][0], GL_STATIC_DRAW);
	}
	if (quad.size() > 0) {
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 4 * quad.size(), &quad[0][0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	return true;
}