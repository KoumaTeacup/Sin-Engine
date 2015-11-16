#include <vector>

#include "vao.h"

#include "SELog.h"
#include "SEVector.h"

using namespace se_data;

SEVAO::SEVAO(std::string name, resourceType type):SEFile(name, type), mode(0), vertexCount(0) {
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
	FILE *f = fopen(filename, "r");
	if (!f) {
#ifdef SE_DEBUG
		SE_LogManager.append(se_debug::LOGTYPE_CONTINUE, "failed.");
		char log[64];
		sprintf(log, "Failed to open file \"%s\".", filename);
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, log);
#endif
		return false;
	}

	// Load VBOs
	std::vector<SEVector3f> pnt_t;
	std::vector<SEVector3f> pnt;
	std::vector<SEVector3f> nrm_t;
	std::vector<SEVector3f> nrm;
	std::vector<SEVector2f> tex_t;
	std::vector<SEVector2f> tex;
	std::vector<SEVector3f> tan;
	SEVector2f input2f;
	SEVector3f input3f;
	SEVector3i input3i;

	char buf[1024];
	int flag = fscanf(f, "%s", buf);
	while (flag!=EOF ) {
		if (strcmp(buf, "#") == 0) fgets(buf, 1024, f);
		else if (strcmp(buf, "g") == 0) fgets(buf, 1024, f);
		else if (strcmp(buf, "s") == 0) fgets(buf, 1024, f);
		else if (strcmp(buf, "v") == 0) {
			fscanf(f, "%f %f %f\n", &input3f[0], &input3f[1], &input3f[2]);
			pnt_t.push_back(input3f);
		}else if (strcmp(buf, "vt") == 0) {
			fscanf(f, "%f %f\n", &input2f[0], &input2f[1]);
			tex_t.push_back(input2f);
		}else if (strcmp(buf, "vn") == 0) {
			fscanf(f, "%f %f %f\n", &input3f[0], &input3f[1], &input3f[2]);
			nrm_t.push_back(input3f);
		}else if (strcmp(buf, "f") == 0) {
			for (int i = 0; i < 4; ++i) {
				fscanf(f, " %d/%d/%d", &input3i[0], &input3i[1], &input3i[2]);
				input3i -= SEVector3i(1, 1, 1);
				pnt.push_back(pnt_t[input3i[0]]);
				tex.push_back(tex_t[input3i[1]]);
				nrm.push_back(nrm_t[input3i[2]]);
				++vertexCount;
			}
		}
		flag = fscanf(f, "%s", buf);
	}
	fclose(f);

	// process data

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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
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

	glDrawArrays(GL_QUADS, 0, vertexCount);

	glBindVertexArray(0);

	// Unbind shader.
	glUseProgram(0);
}