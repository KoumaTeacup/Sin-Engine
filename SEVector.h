/***************************************************************************************************
* - SEVector.h
* - Template Vector Class of Sin Engine
* - This class template provides various kinds of functionality for vector mathmatics needed for
*	graphics.
* -	Note, using the original class, namely "vector<DIM, T>" by DIM beyond the range 2-4 or T of
*	any non-primitive types instead of provided typedefs, namely "SEVectorxx" will introduce 
*	undefined behaviors among member methods.
* -	Do this only when you know what you are doing.
***************************************************************************************************/

#ifndef SEVECTOR_H
#define SEVECTOR_H

#include "SELog.h"

namespace se_data {

template <unsigned DIM, typename T> class matrix;

template <unsigned DIM, typename T>
class vector {
public:
	// default ctor
	vector<DIM, T>() : data() {}

	// copy ctor
	vector<DIM, T>(const vector<DIM, T> &rhs) {	memcpy(data, rhs.data, DIM * sizeof(T));}

	// misc ctors
	vector<DIM, T>(T vx, T vy = 0, T vz = 0, T vw = 0) { initData(vx,vy,vz,vw); }

	// copy assignment
	vector<DIM, T>& operator=(const vector<DIM, T>&) { 
		memcpy(data, rhs.data, DIM * sizeof(T));
		return *this;
	}

	// operation overload
	vector<DIM, T>	operator-()								const;	//negation
	vector<DIM, T>	operator+(const vector<DIM, T> &rhs)	const;	//addition
	vector<DIM, T>	operator-(const vector<DIM, T> &rhs)	const;	//subtraction
	vector<DIM, T>	operator%(const vector<DIM, T> &rhs)	const;	//cross product
	vector<DIM, T>	operator*(T rhs)						const;	//scalar mult.
	T				operator*(const vector<DIM, T> &rhs)	const;	//dot product
	vector<DIM, T>	operator/(T rhs)						const;	//scalar div.
	vector<DIM, T>&	operator+=(const vector<DIM, T> &rhs);
	vector<DIM, T>&	operator-=(const vector<DIM, T> &rhs);
	vector<DIM, T>&	operator*=(T rhs);
	vector<DIM, T>& operator*=(const matrix<DIM, T>&rhs);			//matrix product
	vector<DIM, T>&	operator/=(T rhs);
	T&				operator[](int index);
	T				operator[](int index) const;
	bool			operator==(const vector<DIM, T> &rhs)	const;
	bool			operator!=(const vector<DIM, T> &rhs)	const;

	// member function
	void	unify() { *this /= length(); }
	T		length() const { return sqrt(lengthSqaure()); }
	T		lengthSqaure() const;

	// setter&getter
	void	initData(T vx = 0, T vy = 0, T vz = 0, T vw = 0);

	// friends
	//friend vector<DIM, T> operator*(T lhs, const vector<DIM,T> &rhs) { return rhs * lhs; }
	//friend vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs);

private:
	// member variable
	T data[DIM];
};

/***************************************************************************************************/
/***************************************************************************************************/
// Method definitions

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator-() const {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i) {
		result[i] = -1 * data[i];
	}
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator+(const vector<DIM, T> &rhs) const {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i) {
		result[i] = data[i] + rhs[i];
	}
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator-(const vector<DIM, T> &rhs) const {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i) {
		result[i] = data[i] - rhs[i];
	}
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator%(const vector<DIM, T> &rhs) const {
	vector<DIM, T> result;
#ifdef SE_DEBUG
	if (DIM <= 2) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Cross product performed on non-3d vectors.");
		return result;
	}
#endif
	result[0] = data[1] * rhs[2] - data[2] * rhs[1];
	result[1] = data[2] * rhs[0] - data[0] * rhs[2];
	result[2] = data[0] * rhs[1] - data[1] * rhs[0];
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator*(T rhs) const {
	vector<DIM, T> result;
	for (int i = 0; i < DIM; ++i) {
		result[i] = data[i] * rhs;
	}
	return result;
}

template <unsigned DIM, typename T>
T vector<DIM, T>::operator*(const vector<DIM, T> &rhs) const {
	T result = T();
	for (int i = 0; i < DIM; ++i) {
		result += data[i] * rhs[i];
	}
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T> vector<DIM, T>::operator/(T rhs) const {
	vector<DIM, T> result;
#ifdef SE_DEBUG
	if (rhs == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
		return result;
	}
#endif
	for (int i = 0; i < DIM; ++i) {
		result[i] = data[i] / rhs;
	}
	return result;
}

template <unsigned DIM, typename T>
vector<DIM, T>& vector<DIM, T>::operator+=(const vector<DIM, T> &rhs) {
	for (int i = 0; i < DIM; ++i) {
		data[i] += rhs[i];
	}
	return *this;
}

template <unsigned DIM, typename T>
vector<DIM, T>& vector<DIM, T>::operator-=(const vector<DIM, T> &rhs) {
	for (int i = 0; i < DIM; ++i) {
		data[i] -= rhs[i];
	}
	return *this;
}

template <unsigned DIM, typename T>
vector<DIM, T>& vector<DIM, T>::operator*=(T rhs) {
	for (int i = 0; i < DIM; ++i) {
		data[i] *= rhs;
	}
	return *this;
}

template <unsigned DIM, typename T>
vector<DIM, T>& vector<DIM, T>::operator/=(T rhs) {
#ifdef SE_DEBUG
	if (rhs == 0) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
		return *this
	}
#endif
	for (int i = 0; i < DIM; ++i)
		data[i] /= rhs;
	return *this;
}

template <unsigned DIM, typename T>
bool vector<DIM, T>::operator==(const vector<DIM, T> &rhs) const {
	for (int i = 0; i < DIM; ++i) {
		if (data[i] != rhs[i]) return false;
	}
	return true;
}

template <unsigned DIM, typename T>
bool vector<DIM, T>::operator!=(const vector<DIM, T> &rhs) const {
	return !(*this == rhs);
}

template<unsigned DIM, typename T>
T vector<DIM, T>::lengthSqaure() const
{
	T result = T();
	for (int i = 0; i < DIM; ++i)
		result += data[i] * data[i];
	return result;
}

template <unsigned DIM, typename T>
void vector<DIM, T>::initData(T vx, T vy, T vz, T vw) {
	if (DIM>0) data[0] = vx;
	if (DIM>1) data[1] = vy;
	if (DIM>2) data[2] = vz;
	if (DIM>3) data[3] = vw;
}

template <unsigned DIM, typename T>
T& vector<DIM, T>::operator[](int index) {
#ifdef SE_DEBUG
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Vector index out of range.");
		index = 0;
	}
#endif
	return data[index];
}

template <unsigned DIM, typename T>
T vector<DIM, T>::operator[](int index) const {
#ifdef SE_DEBUG
	if (index > DIM) {
		SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Vector index out of range.");
		index = 0;
	}
#endif
	return data[index];
}

}// namespace se_debug

 // typedefs
typedef	se_data::vector<2, float> SEVector2f;
typedef se_data::vector<3, float> SEVector3f;
typedef se_data::vector<4, float> SEVector4f;
typedef se_data::vector<2, double> SEVector2d;
typedef	se_data::vector<3, double> SEVector3d;
typedef	se_data::vector<4, double> SEVector4d;
typedef	se_data::vector<2, int> SEVector2i;
typedef	se_data::vector<3, int> SEVector3i;
typedef	se_data::vector<4, int> SEVector4i;
typedef	se_data::vector<2, unsigned> SEVector2ui;
typedef	se_data::vector<3, unsigned> SEVector3ui;
typedef	se_data::vector<4, unsigned> SEVector4ui;
typedef	se_data::vector<2, char> SEVector2c;
typedef	se_data::vector<3, char> SEVector3c;
typedef	se_data::vector<4, char> SEVector4c;
typedef	se_data::vector<2, unsigned char> SEVector2uc;
typedef	se_data::vector<3, unsigned char> SEVector3uc;
typedef	se_data::vector<4, unsigned char> SEVector4uc;

#endif