#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdlib.h>
#include <string.h>

template<class T> class Vector {

public:
	Vector(const int &size) : 
	_size(size), 
	_container(0x0) {
		_container = (T *) malloc(size * sizeof(T));
		memset(_container, 0x0, size * sizeof(T));
	}

	~Vector() {
		if (_container != 0x0) {
			free(_container);
		}
	}

	T &operator[](const int &index) {
		return _container[index];
	}

private:

	int _size;

	T *_container;
	
};

#endif