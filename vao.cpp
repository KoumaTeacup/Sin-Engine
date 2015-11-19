#include <vector>
#include <fstream>

#include "vao.h"

#include "SELog.h"
#include "SEVector.h"

using namespace se_data;

SEVAO::SEVAO(std::string name):SEFile(name, RESTYPE_VERTEX_ARRAY), vertexCount(0) {
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

	char *pch;
	int mode = 3;
	int modeCurr = 0;

	char buf[1024];
	while (ifs.peek() != EOF) {
		ifs >> buf;
		if (buf[0] == '#') ifs.getline(buf, 1024);
		else if (strcmp(buf, "g") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "s") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "usemtl") == 0) ifs.getline(buf, 1024);
		else if (strcmp(buf, "v") == 0) {
			ifs >> input3f[0] >> input3f[1] >> input3f[2];
			ifs.getline(buf, 1024);
			pnt_t.push_back(input3f);
		}
		else if (strcmp(buf, "vt") == 0) {
			ifs >> input2f[0] >> input2f[1];
			ifs.getline(buf, 1024);
			tex_t.push_back(input2f);
		}
		else if (strcmp(buf, "vn") == 0) {
			ifs >> input3f[0] >> input3f[1] >> input3f[2];
			ifs.getline(buf, 1024);
			nrm_t.push_back(input3f);
		}
		else if (strcmp(buf, "f") == 0) {
			ifs.getline(buf, 1024);
			char str[5][32];
			memset(str, 0, sizeof(char) * 5 * 32);
			pch = strtok(buf, " "); 
			modeCurr = 0;
			while (pch) {
				strcpy(str[modeCurr++], pch);
				pch = strtok(NULL, " ");
			}
			for (int j = 0; j < modeCurr; ++j) {
				//int face = fscanf(f, " %d/%d/%d", &input3i[0], &input3i[1], &input3i[2]);
				//fscanf(f, " %d/%d", &input2i[0], &input2i[1]);
				pch = strtok(str[j], "/");
				int i = 0;
				while (pch) {
					input3i[i++] = std::stoi(pch);
					pch = strtok(NULL, "/");
				}
				input3i -= SEVector3i(1, 1, 1);
				if (input3i[0] >= 0) pnt.push_back(pnt_t[input3i[0]]);
				if (input3i[1] >= 0) tex.push_back(tex_t[input3i[1]]);
				if (input3i[2] >= 0) nrm.push_back(nrm_t[input3i[2]]);
				++vertexCount;
			}
			if (modeCurr != mode) {
				modeIndex.push_back(vertexCount-modeCurr);
				mode = modeCurr;
				vertexCount = 0;
			}
		}
	}
	modeIndex.push_back(vertexCount - modeCurr);
	ifs.close();

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
	int j = 0;
	for (auto i = modeIndex.begin(); i != modeIndex.end(); ++i) {
		glDrawArrays(GL_TRIANGLES, j, *i);
		j += *i;
		if (++i == modeIndex.end()) break;
		glDrawArrays(GL_QUADS, j, *i);
		j += *i;
	}
}

void SEVAO::onPostUpdate() {
	glBindVertexArray(0);
}