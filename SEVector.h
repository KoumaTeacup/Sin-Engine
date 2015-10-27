#ifndef SEVECTOR_H
#define SEVECTOR_H

#include <stdlib.h> //memset()

template <typename T>
class vector{
public:
	//member variables
	T data[];

	//ctors
	vector<T>(int d)				: dim(d), data(T[d]) {};
	vector<T>(int d, T val)			: dim(d) { memset(data, val, dim); }

	//copy ctor
	vector(const vector &obj)	: dim(obj.dim), data(obj.data) {};

	//dtor
	~vector<T>();

	//assignment operator
	vector		&operator=(const vector &rhs);

	//operation overload
	vector<T>	operator-()							const;	//negation
	vector<T>	operator+(const vector<T> &rhs)		const;	//addition
	vector<T>	operator-(const vector<T> &rhs)		const;	//subtraction
	vector<T>	operator%(const vector<T> &rhs)		const;	//cross product
	vector<T>	operator*(T rhs)					const;	//scalar mult.
	T&			operator*(const vector<T> &rhs)		const;	//dot product
	vector<T>	operator/(T rhs)					const;	//scalar div.
	vector<T>&	operator+=(const vector<T> &rhs);
	vector<T>&	operator-=(const vector<T> &rhs);
	vector<T>&	operator*=(T rhs);
	vector<T>&	operator/=(T rhs);
	T&			operator[](int index) { return data[index]; }
	bool		operator==(const vector<T> &rhs)	const;
	bool		operator!=(const vector<T> &rhs)	const;
	
	//member function
	void		unify();
	T			length() const;

private:
	//hidden default ctor.
	vector<T>();

	//member variable
	int dim;
};

#endif