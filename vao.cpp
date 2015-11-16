#include <fstream>
#include <vector>

#include "vao.h"

#include "SELog.h"
#include "SEVector.h"

using namespace se_data;

SEVAO::SEVAO(std::string name, resourceType type):SEFile(name, type), mode(0) {
}

SEVAO::~SEVAO() {
	glDeleteVertexArrays(1, &id);
}

bool SEVAO::load(const char* filename) {
#ifdef SE_DEBUG
	char log[64];
	sprintf(log, "Loading VAO file %s...", filename);
	SE_LogManager.append(se_debug::LOGTYPE_GENERAL, log);
#endif
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
#ifdef SE_DEBUG
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "failed.");
		char log[64];
		sprintf(log, "Failed to open file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
		return false;
	}

	// Load VBOs
	std::vector<SEVector3f> pnt;
	std::vector<SEVector3f> nrm;
	std::vector<SEVector2f> tex;
	std::vector<SEVector3f> tan;
	std::vector<SEVector3i> tri;
	std::vector<SEVector4i> quad;
	SEVector2f input2f;
	SEVector3f input3f;
	SEVector3i input3i;
	SEVector4i input4i;

	char buf[64];
	char sentry = ifs.peek();
	while (sentry != EOF) {
		ifs.getline(buf, 64);
		if (buf[0] != '#') continue;
		do {
			if (strcmp(buf, "#pnt")==0) {
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				pnt.push_back(input3f);
			}
			else if (strcmp(buf, "#nrm")==0) {
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				nrm.push_back(input3f);
			}
			else if (strcmp(buf, "#tex")==0) {
				ifs >> input2f[0] >> input2f[1];
				tex.push_back(input2f);
			}
			else if (strcmp(buf, "#tan")==0) {
				ifs >> input3f[0] >> input3f[1] >> input3f[2];
				tan.push_back(input3f);
			}
			else if (strcmp(buf, "#tri")==0) {
				ifs >> input3i[0] >> input3i[1] >> input3i[2];
				tri.push_back(input3i);
			}
			else if (strcmp(buf, "#quad")==0) {
				ifs >> input4i[0] >> input4i[1] >> input4i[2] >> input4i[3];
				quad.push_back(input4i);
			}
			sentry = ifs.ignore().peek();
		} while (sentry != EOF && sentry != '#');
	}
	ifs.close();

	// Generate VAO.
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);

	GLuint vbo;
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
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (tex.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * tex.size(), &tex[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (tan.size() > 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * tan.size(), &tan[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	int *pIndices;
	// Generate IBO.
	if (tri.size() > 0) {
		mode = 3;
		indicesCount = tri.size();
		pIndices = &tri[0][0];
	}
	if (quad.size() > 0) {
		mode = 4;
		indicesCount = quad.size();
		pIndices = &quad[0][0];
	}
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mode * indicesCount, pIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#ifdef SE_DEBUG
	SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "success.");
#endif;
	return true;
}

void SEVAO::onInit(){
}

void SEVAO::onRelease() {
}

void SEVAO::onDraw() {
	glBindVertexArray(id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, indicesCount * mode, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind shader.
	glUseProgram(0);
}