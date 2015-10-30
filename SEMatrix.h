/***************************************************************************************************
* - SEMatrix.h
* - Template Matrix Class of Sin Engine
* - This class template provides various kinds of functionality for matrix mathmatics needed for
*	graphics.
* -	See SEvector.h
***************************************************************************************************/

#ifndef SEMATRIX_H
#define SEMATRIX_H

#include "SEVector.h"

#define SE_MATRIX_TRANSLATE4(...)	se_data::matrix<4,float>::TL(__VA_ARGS__)
#define SE_MATRIX_ROTATE4(...)		se_data::matrix<4,float>::RT(__VA_ARGS__)
#define SE_MATRIX_SCALE4(...)		se_data::matrix<4,float>::SL(__VA_ARGS__)
#define SE_MATRIX_PROJECT4(...)		se_data::matrix<4,float>::PJ(__VA_ARGS__)
#define SE_MATRIX_TRANSLATE3(...)	se_data::matrix<3,float>::TL(__VA_ARGS__)
#define SE_MATRIX_ROTATE3(...)		se_data::matrix<3,float>::RT(__VA_ARGS__)
#define SE_MATRIX_SCALE3(...)		se_data::matrix<3,float>::SL(__VA_ARGS__)

namespace se_data {

enum axis {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

template <unsigned DIM, typename T>
class matrix {
public:
	// ctor
	matrix<DIM, T>() {}

	// ctors
	matrix<DIM, T>(
		vector<DIM, T> v0,
		vector<DIM, T> v1 = vector<DIM, T>(),
		vector<DIM, T> v2 = vector<DIM, T>(),
		vector<DIM, T> v3 = vector<DIM, T>()) { initData(v0, v1, v2, v3); }

	matrix<DIM, T>(T m00) {data[0][0] = m00; }
	matrix<DIM, T>(
		T m00, T m01,
		T m10, T m11) {
		data[0].initData(m00, m01);
		data[1].initData(m10, m11);
	}
	matrix<DIM, T>(
		T m00, T m01, T m02,
		T m10, T m11, T m12,
		T m20, T m21, T m22) {
		data[0].initData(m00, m01, m02);
		data[1].initData(m10, m11, m12);
		data[1].initData(m20, m21, m22);
	}
	matrix<DIM, T>(
		T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33) {
		data[0].initData(m00, m01, m02, m03);
		data[1].initData(m10, m11, m12, m13);
		data[1].initData(m20, m21, m22, m23);
		data[1].initData(m30, m31, m32, m33);
	}

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
	void initData(
		vector<DIM,T> v0 = vector<DIM, T>(),
		vector<DIM, T> v1 = vector<DIM, T>(),
		vector<DIM, T> v2 = vector<DIM, T>(),
		vector<DIM, T> v3 = vector<DIM, T>());

	// friends
	//friend matrix<DIM, T> operator*(T lhs, const matrix<DIM, T> &rhs) { return rhs * lhs; }
	//friend vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs);

private:
	// member variable
	vector<DIM, T> data[DIM];
};

/***************************************************************************************************/
/***************************************************************************************************/
// Method definitions

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator*(T val) const {
	matrix<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		result[i] *= val;
	return result;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator/(T val) const {
	matrix<DIM, T> result;
	if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
	else
		for (int i = 0; i < DIM; ++i) {
			result[i] /= val;
		}
	return result;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::operator*(const matrix<DIM, T> &rhs) const {
	matrix<DIM, T> result;
	matrix<DIM, T> trans = rhs.transpose();
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j) {
			result[i][j] = data[i] * trans[j];
		}
	return result;
}

template <unsigned DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator*=(T val) {
	for (int i = 0; i < DIM; ++i) {
		data[i] *= val;
	}
	return *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator*=(const matrix<DIM, T> &rhs) {
	*this = *this * rhs;
	return *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T>& matrix<DIM, T>::operator/=(T val) {
	if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
	else
	for (int i = 0; i < DIM; ++i) {
		data[i] /= val;
	}
	return *this;
}

template <unsigned DIM, typename T>
vector<DIM, T> matrix<DIM, T>::operator[](int index) const {
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Matrix index out of range.");
		index = 0;
	}
	return data[index];
}

template <unsigned DIM, typename T>
vector<DIM, T>& matrix<DIM, T>::operator[](int index) {
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Matrix index out of range.");
		index = 0;
	}
	return data[index];
}

template <unsigned DIM, typename T>
bool matrix<DIM, T>::operator==(const matrix<DIM, T> &rhs) const {
	for (int i = 0; i < DIM; ++i) {
		if (data[i] != rhs) return false;
	}
	return true;
}

template <unsigned DIM, typename T>
bool matrix<DIM, T>::operator!=(const matrix<DIM, T> &rhs) const {
	return !(*this == rhs);
}

template <unsigned DIM, typename T>
void matrix<DIM, T>::identify(T val) {
	for (int i = 0; i < DIM; ++i) {
		data[i][i] = val;
	}
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::transpose() const {
	matrix<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j) {
			result[i][j] = data[j][i];
		}
	return result;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::translate(float x, float y, float z) const {
	return TL(x, y, z) * *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::rotate(axis a, float degree) const {
	return RT(a, degree) * *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::scale(float x, float y, float z) const {
	return SL(x, y, z)* *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T> matrix<DIM, T>::project(float front, float back, float rx, float ry) const {
	return PJ(front, back, rx, ry)* *this;
}

template <unsigned DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::TL(float x, float y, float z) {
	matrix<DIM, T> result;
	result.identify();
	if(DIM>0) result[0][DIM-1] = x;
	if(DIM>1) result[1][DIM-1] = y;
	if(DIM>2) result[2][DIM-1] = z;
	return result;
}

template <unsigned DIM, typename T>
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

template <unsigned DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::SL(float x, float y, float z) {
	matrix<DIM, T> result;
	if(DIM>0) result[0][0] = x;
	if(DIM>1) result[1][1] = y;
	if(DIM>2) result[2][2] = z;
	result[DIM - 1][DIM - 1] = 1;
	return result;
}

template <unsigned DIM, typename T>
matrix<DIM, T>	matrix<DIM, T>::PJ(float front, float back, float rx, float ry) {
	matrix<DIM, T> result;
	if (DIM != 4) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Projection only allowed in 3d space.");
	else {
		result[0][0] = 1 / rx;
		result[1][1] = 1 / ry;
		result[2][2] = (front + back) / (front - back);
		result[2][3] = 2 * front*back / (front - back);
		result[3][2] = -1;
	}
	return result;
}

template <unsigned DIM, typename T>
void matrix<DIM, T>::initData(
	vector<DIM, T> v0 = vector<DIM, T>(),
	vector<DIM, T> v1 = vector<DIM, T>(),
	vector<DIM, T> v2 = vector<DIM, T>(),
	vector<DIM, T> v3 = vector<DIM, T>()) {
	if (DIM>0) data[0] = v0;
	if (DIM>1) data[1] = v1;
	if (DIM>2) data[2] = v2;
	if (DIM>3) data[3] = v3;
}

template <unsigned DIM, typename T>
vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs) {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i)
		result[i] = lhs[i] * rhs;
	return result;
}

template <unsigned DIM, typename T>
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

