#ifndef SEVECTOR_H
#define SEVECTOR_H

#include "SELog.h"

namespace se_data {

	template <int DIM, typename T> class matrix;

	template <int DIM, typename T>
	class vector {
	public:
		// ctor
		vector<DIM, T>() : data() {}

		// ctors
		vector<DIM, T>(T val) { initData(val); }

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
		void	initData(T val = 0);

		// friends
		//friend vector<DIM, T> operator*(T lhs, const vector<DIM,T> &rhs) { return rhs * lhs; }
		//friend vector<DIM, T> operator*(const matrix<DIM, T> &lhs, const vector<DIM, T> &rhs);

	private:
		// member variable
		T data[4];
	};

	/*************************************************************************************/
	/*************************************************************************************/
	// Method definitions

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator-() const {
		vector<DIM, T> result;
		for (int i = 0; i < DIM; ++i) {
			result[i] = -1 * data[i];
		}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator+(const vector<DIM, T> &rhs) const {
		vector<DIM, T> result;
		for (int i = 0; i < DIM; ++i) {
			result[i] = data[i] + rhs[i];
		}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator-(const vector<DIM, T> &rhs) const {
		vector<DIM, T> result;
		for (int i = 0; i < DIM; ++i) {
			result[i] = data[i] - rhs[i];
		}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator%(const vector<DIM, T> &rhs) const {
		vector<DIM, T> result;
		if (DIM == 2) {
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Cross product performed on 2d vectors.");
		}
		else {
			result[0] = data[1] * rhs[2] - data[2] * rhs[1];
			result[1] = data[2] * rhs[0] - data[0] * rhs[2];
			result[2] = data[0] * rhs[1] - data[1] * rhs[0];
		}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator*(T rhs) const {
		vector<DIM, T> result;
		for (int i = 0; i < DIM; ++i) {
			result[i] = data[i] * rhs;
		}
		return result;
	}

	template <int DIM, typename T>
	T vector<DIM, T>::operator*(const vector<DIM, T> &rhs) const {
		T result = T();
		for (int i = 0; i < DIM; ++i) {
			result += data[i] * rhs[i];
		}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T> vector<DIM, T>::operator/(T rhs) const {
		vector<DIM, T> result;
		if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
		else
			for (int i = 0; i < DIM; ++i) {
				result[i] = data[i] / rhs;
			}
		return result;
	}

	template <int DIM, typename T>
	vector<DIM, T>& vector<DIM, T>::operator+=(const vector<DIM, T> &rhs) {
		for (int i = 0; i < DIM; ++i) {
			data[i] += rhs[i];
		}
		return *this;
	}

	template <int DIM, typename T>
	vector<DIM, T>& vector<DIM, T>::operator-=(const vector<DIM, T> &rhs) {
		for (int i = 0; i < DIM; ++i) {
			data[i] -= rhs[i];
		}
		return *this;
	}

	template <int DIM, typename T>
	vector<DIM, T>& vector<DIM, T>::operator*=(T rhs) {
		for (int i = 0; i < DIM; ++i) {
			data[i] *= rhs;
		}
		return *this;
	}

	template <int DIM, typename T>
	vector<DIM, T>& vector<DIM, T>::operator/=(T rhs) {
		if (rhs == 0) SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Division by zero, no action taken.");
		else
			for (int i = 0; i < DIM; ++i) {
				data[i] /= rhs;
			}
		return *this;
	}

	template <int DIM, typename T>
	bool vector<DIM, T>::operator==(const vector<DIM, T> &rhs) const {
		for (int i = 0; i < DIM; ++i) {
			if (data[i] != rhs[i]) return false;
		}
		return true;
	}

	template <int DIM, typename T>
	bool vector<DIM, T>::operator!=(const vector<DIM, T> &rhs) const {
		return !(*this == rhs);
	}

	template<int DIM, typename T>
	T vector<DIM, T>::lengthSqaure() const
	{
		T result = T();
		for (int i = 0; i < DIM; ++i)
			result += data[i] * data[i];
		return result;
	}

	template <int DIM, typename T>
	void vector<DIM, T>::initData(T val) {
		for (int i = 0; i < DIM; ++i) {
			data[i] = val;
		}
	}

	template <int DIM, typename T>
	T& vector<DIM, T>::operator[](int index) {
		if (index > DIM) {
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Vector index out of range.");
			index = 0;
		}
		return data[index];
	}

	template <int DIM, typename T>
	T vector<DIM, T>::operator[](int index) const {
		if (index > DIM) {
			SE_LogManager.append(se_debug::LOGTYPE_ERROR, "Vector index out of range.");
			index = 0;
		}
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