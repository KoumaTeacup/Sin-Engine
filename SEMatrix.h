#ifndef SEMATRIX_H
#define SEMATRIX_H

#include "SEVector.h"

#define SE_MATRIX_TRANSLATE4(...)	se_data::matrix<4,float>::TL(__VA_ARGS__);
#define SE_MATRIX_ROTATE4(...)		se_data::matrix<4,float>::RT(__VA_ARGS__);
#define SE_MATRIX_SCALE4(...)		se_data::matrix<4,float>::SL(__VA_ARGS__);
#define SE_MATRIX_PROJECT4(...)		se_data::matrix<4,float>::PJ(__VA_ARGS__);
#define SE_MATRIX_TRANSLATE3(...)	se_data::matrix<3,float>::TL(__VA_ARGS__);
#define SE_MATRIX_ROTATE3(...)		se_data::matrix<3,float>::RT(__VA_ARGS__);
#define SE_MATRIX_SCALE3(...)		se_data::matrix<3,float>::SL(__VA_ARGS__);

namespace se_data {

enum axis {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

template <int DIM, typename T>
class matrix {
public:
	// ctor
	matrix<DIM, T>() {}

	// ctors
	matrix<DIM, T>(T val) { initData(val); }

	// operation overload
	matrix<DIM, T>	operator*(T val)						const;
	matrix<DIM, T>	operator/(T val)						const;
	matrix<DIM, T>	operator*(const matrix<DIM, T> &rhs)	const;
	matrix<DIM, T>&	operator*=(T val);
	matrix<DIM, T>& operator*=(const matrix<DIM, T> &rhs);
	matrix<DIM, T>&	operator/=(T val);
	vector<DIM, T>	operator[](int index)					const;
	vector<DIM, T>&	operator[](int index);
	bool			operator==(const matrix<DIM, T> &rhs)	const;
	bool			operator!=(const matrix<DIM, T> &rhs)	const;

	// member function
	void			identify(T val = 1);
	matrix<DIM, T>	transpose() const;
	matrix<DIM, T>	translate(float x = 0, float y = 0, float z = 0) const;
	matrix<DIM, T>	rotate(axis a, float degree) const;
	matrix<DIM, T>	scale(float x=1, float y=1, float z=1) const;
	matrix<DIM, T>	project(float front, float back, float rx, float ry) const;

	// matrix generation
	static matrix<DIM, T>	TL(float x = 0, float y = 0, float z = 0);
	static matrix<DIM, T>	RT(axis a = AXIS_Z, float degree = 0);
	static matrix<DIM, T>	SL(float x = 0, float y = 0, float z = 0);
	static matrix<DIM, T>	PJ(float front, float back, float rx, float ry);

	// setter&getter
	void initData(T val = 0);

	// friends
	//friend matrix<DIM, T> operator*(T lhs, const matrix<DIM, T> &rhs) { return rhs * lhs; }
	//friend vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs);

private:
	// member variable
	vector<DIM, T> data[4];
};

/*************************************************************************************/
/*************************************************************************************/
// Method definitions

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator*(T val) const {
	matrix<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		result[i] *= val;
	return result;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator/(T val) const {
	matrix<DIM, T> result;
	if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
	else
		for (int i = 0; i < DIM; ++i) {
			result[i] /= val;
		}
	return result;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator*(const matrix<DIM, T> &rhs) const {
	matrix<DIM, T> result;
	matrix<DIM, T> trans = rhs.transpose();
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j) {
			result[i][j] = data[i] * trans[j];
		}
	return result;
}

template <int DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator*=(T val) {
	for (int i = 0; i < DIM; ++i) {
		data[i] *= val;
	}
	return *this;
}

template <int DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator*=(const matrix<DIM, T> &rhs) {
	*this = *this * rhs;
	return *this;
}

template <int DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator/=(T val) {
	if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
	else
	for (int i = 0; i < DIM; ++i) {
		data[i] /= val;
	}
	return *this;
}

template <int DIM, typename T>
vector<DIM, T> matrix<DIM, T>::operator[](int index) const {
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Matrix index out of range.");
		index = 0;
	}
	return data[index];
}

template <int DIM, typename T>
vector<DIM, T>& matrix<DIM, T>::operator[](int index) {
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Matrix index out of range.");
		index = 0;
	}
	return data[index];
}

template <int DIM, typename T>
bool matrix<DIM, T>::operator==(const matrix<DIM, T> &rhs) const {
	for (int i = 0; i < DIM; ++i) {
		if (data[i] != rhs) return false;
	}
	return true;
}

template <int DIM, typename T>
bool matrix<DIM, T>::operator!=(const matrix<DIM, T> &rhs) const {
	return !(*this == rhs);
}

template <int DIM, typename T>
void matrix<DIM, T>::identify(T val) {
	for (int i = 0; i < DIM; ++i) {
		data[i][i] = val;
	}
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::transpose() const {
	matrix<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j) {
			result[i][j] = data[j][i];
		}
	return result;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::translate(float x, float y, float z) const {
	return TL(x, y, z) * *this;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::rotate(axis a, float degree) const {
	return RT(a, degree) * *this;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::scale(float x, float y, float z) const {
	return SL(x, y, z)* *this;
}

template <int DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::project(float front, float back, float rx, float ry) const {
	return PJ(front, back, rx, ry)* *this;
}

template <int DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::TL(float x, float y, float z) {
	matrix<DIM, T> result;
	result.identify();
	result[0][DIM-1] = x;
	result[1][DIM-1] = y;
	result[2][DIM-1] = z;
	result[DIM - 1][DIM - 1] = 1.0f;
	return result;
}

template <int DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::RT(axis a, float degree) {
	matrix<DIM, T> result;
	result.identify();
	if (DIM < 3) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Invalid matrix dimension for rotation.");
		return result;
	}
	if (DIM == 3) {
		result[0][0] = cosf(degree);
		result[0][1] = -sinf(degree);
		result[1][0] = sinf(degree);
		result[1][1] = cosf(degree);
		return result;
	}
	switch (a) {
	case AXIS_X:
		result[1][1] = cosf(degree);
		result[1][2] = -sinf(degree);
		result[2][1] = sinf(degree);
		result[2][2] = cosf(degree);
		break;
	case AXIS_Y:
		result[0][0] = cosf(degree);
		result[0][2] = sinf(degree);
		result[2][0] = -sinf(degree);
		result[2][2] = cosf(degree);
		break;
	case AXIS_Z:
		result[0][0] = cosf(degree);
		result[0][1] = -sinf(degree);
		result[1][0] = sinf(degree);
		result[1][1] = cosf(degree);
		break;
	}
	return result;
}

template <int DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::SL(float x, float y, float z) {
	matrix<DIM, T> result;
	result[0][0] = x;
	result[1][1] = y;
	result[2][2] = z;
	result[DIM - 1][DIM - 1] = 1;
	return result;
}

template <int DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::PJ(float front, float back, float rx, float ry) {
	if (DIM != 4) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Projection only allowed in 3d space.");
	else {
		matrix<DIM, T> result;
		result[0][0] = 1 / rx;
		result[1][1] = 1 / ry;
		result[2][2] = (front + back) / (front - back);
		result[2][3] = 2 * front*back / (front - back);
		result[3][2] = -1;
	}
	return result;
}

template <int DIM, typename T>
void matrix<DIM, T>::initData(T val) {
	for (int i = 0; i < DIM; ++i) {
		data[i] = vector<DIM, T>(val);
	}
}

template <int DIM, typename T>
vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs) {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		result[i] = lhs[i] * rhs;
	return result;
}

template <int DIM, typename T>
vector<DIM, T>& vector<DIM,T>::operator*=(const matrix<DIM, T> &rhs) {
	*this = rhs * *this;
	return *this;
}

}// namespace se_data

// typedefs
typedef se_data::matrix<2, float> SEMatrix2f;
typedef se_data::matrix<3, float> SEMatrix3f;
typedef se_data::matrix<4, float> SEMatrix4f;
typedef se_data::matrix<2, double> SEMatrix2d;
typedef se_data::matrix<3, double> SEMatrix3d;
typedef se_data::matrix<4, double> SEMatrix4d;
typedef se_data::matrix<2, int> SEMatrix2i;
typedef se_data::matrix<3, int> SEMatrix3i;
typedef se_data::matrix<4, int> SEMatrix4i;
typedef se_data::matrix<2, unsigned> SEMatrix2ui;
typedef se_data::matrix<3, unsigned> SEMatrix3ui;
typedef se_data::matrix<4, unsigned> SEMatrix4ui;
typedef se_data::matrix<2, char> SEMatrix2c;
typedef se_data::matrix<3, char> SEMatrix3c;
typedef se_data::matrix<4, char> SEMatrix4c;
typedef se_data::matrix<2, unsigned char> SEMatrix2uc;
typedef se_data::matrix<3, unsigned char> SEMatrix3uc;
typedef se_data::matrix<4, unsigned char> SEMatrix4uc;

#endif

