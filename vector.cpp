#include <iostream>
#include <cstddef>
#include <vector>
#define N 10

class MyIntVector {
public:
	MyIntVector();
	explicit MyIntVector(size_t size, int default_value = 0);
	MyIntVector(const MyIntVector& other);
	MyIntVector& operator=(const MyIntVector& other);
	~MyIntVector();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	void resize(size_t new_size, int default_value = 0);
	void push_back(int value);
	void reserve(size_t size);
	void shrink_to_fit();

	int operator[](size_t index) const;
	int& operator[](size_t index);

	int at(size_t index) const;
	int& at(size_t index);

private:
	int* _vec;
	size_t _size;
	size_t _capacity;
	void definition(size_t size, size_t capacity);
	void copy(size_t size, size_t capacity);
};

void MyIntVector::definition(size_t size, size_t capacity) {
	_size = size;
	_capacity = capacity;
	_vec = new int[capacity];
}

void MyIntVector::copy(size_t size, size_t capacity) {
	int* old_vec = _vec;
	definition(size, capacity);
	for (int i = 0; i < _size; ++i) _vec[i] = old_vec[i];
	delete[] old_vec;
}

MyIntVector::MyIntVector() {
	definition(0, 0);
}

MyIntVector::MyIntVector(size_t size, int default_value) {
	definition(size, size);
	for (int i = 0; i < _size; ++i) _vec[i] = default_value;
}

MyIntVector::MyIntVector(const MyIntVector& other) {
	definition(other._size, other._size);
	memcpy(_vec, other._vec, other._size * sizeof(int));
}

MyIntVector& MyIntVector::operator=(const MyIntVector& other) {
	if (&other != this) {
		definition(other._size, other._size);
		memcpy(_vec, other._vec, other._size * sizeof(int));
	}
	return *this;
}

MyIntVector::~MyIntVector() {
	delete[] _vec;
}

size_t MyIntVector::size() const {
	return _size;
}

size_t MyIntVector::capacity() const {
	return _capacity;
}

bool MyIntVector::empty() const {
	return _size == 0;
}

void MyIntVector::resize(size_t new_size, int default_value) {
	if (new_size > _size) {
		reserve(new_size+N);
		for (int i = _size; i < new_size; ++i) _vec[i] = default_value;
		_size = new_size;
	}
	else {
		copy(new_size, _capacity);
	}
}


void MyIntVector::push_back(int value) {
	resize(_size + 1, value);

}


void MyIntVector::reserve(size_t size) {
	if (size > _capacity) {
		copy(_size, size);
	}
}


void MyIntVector::shrink_to_fit() {
	copy(_size, _size);
}


int& MyIntVector:: operator[](size_t index) {
	return _vec[index];
}

int MyIntVector:: operator[](size_t index) const {
	return _vec[index];
}

int MyIntVector::at(size_t index) const {
	if (index >= _size) {
		throw std::out_of_range("");
	}
	return _vec[index];
}

int& MyIntVector::at(size_t index) {
	if (index >= _size) {
		throw std::out_of_range("");
	}
	return _vec[index];
}

std::ostream& operator<<(std::ostream& os, const MyIntVector& v) {
	for (int i = 0; i < v.size(); i++) {
		os << v[i] << " ";
	}
	return os;
}


int main()
{
	MyIntVector v(2);
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;
	v.push_back(2);
	std::cout << "Push back" << std::endl;
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;
	v.reserve(15);
	std::cout << " Reserve 15" << std::endl;
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;
	v[0] = 2;
	v.resize(2);
	std::cout << "v[0]=2 + Resize 2" << std::endl;
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;
	//std::cout << v[2];

	//std::vector<int> a = { 1, 2, 3 };
	//std::cout << a.capacity();
	//a.resize(1);
	//std::cout << a.capacity();

	MyIntVector g(v);
	std::cout << "Copy g" << std::endl;
	std::cout << g << std::endl;
	std::cout << "Capacity: " << g.capacity() << ", Size: " << g.size() << std::endl;

	v.shrink_to_fit();
	std::cout << "Shrink_to_fit" << std::endl;
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;

	v.push_back(3);
	std::cout << "Push back" << std::endl;
	std::cout << v << std::endl;
	std::cout << "Capacity: " << v.capacity() << ", Size: " << v.size() << std::endl;
}

