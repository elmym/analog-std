#include <iostream>
#define CHECK(x) if (x) {} else {errors(done, __LINE__);}

class A {
public:
	A(int* increase_at_destruct) : _increase_at_destruct(increase_at_destruct) {}
	~A() { (*_increase_at_destruct)++; }

	void change_value() { _value++; }
	int value() const { return _value; }
private:
	int* _increase_at_destruct;
	int _value = 0;
};

class UniquePtrA {
public:
	UniquePtrA(A* p = nullptr);
	~UniquePtrA();
	UniquePtrA(UniquePtrA&& other) noexcept;
	UniquePtrA(const UniquePtrA&) = delete;
	UniquePtrA& operator = (const UniquePtrA&) = delete;
	A* get() const;
	A* operator->() const;
	A* release();
	void reset(A* ptr = nullptr);
private:
	void _delete();
	A* _a;
};

UniquePtrA::UniquePtrA(A* ptr) : _a(ptr) {}

UniquePtrA::UniquePtrA(UniquePtrA&& other) noexcept: _a(other._a) {
	other._a = nullptr;
}

UniquePtrA::~UniquePtrA() {
	_delete();
}

void UniquePtrA::_delete() {
	delete _a;
}

A* UniquePtrA::get() const {
	return _a;
}

A* UniquePtrA::operator->() const {
	return _a;
}


A* UniquePtrA::release() {
	A* ptr = _a;
	_a = nullptr;
	return ptr;
}

void UniquePtrA::reset(A* ptr) {
	if (_a == ptr)
		delete ptr;
	else {
		_delete();
		_a = ptr;
	}
}



void errors(int& done, int line) {
	done++;
	std::cout << "error in " << line << std::endl;
}

int main() {
	int done = 0;
	int n_destructs = 0;
	UniquePtrA a(new A(&n_destructs));
	A* ptr = a.get();
	ptr->change_value();
	CHECK(a->value() == 1);

	//UniquePtrA b = a;   //should not compile

	A* ptr1 = a.release();
	CHECK(a.get() == nullptr);
	CHECK(ptr1 == ptr);
	CHECK(ptr->value() == 1);

	a.reset(ptr1);
	a->change_value();
	CHECK(a->value() == 2);

	UniquePtrA c(std::move(a));
	CHECK(a.get() == nullptr);
	CHECK(n_destructs == 0);
	CHECK(c->value() == 2);

	c.reset();
	CHECK(n_destructs == 1);
	CHECK(c.get() == nullptr);

	if (done == 0) {
		std::cout << "DONE" << std::endl;
	}
	else {
		std::cout << "Count of errors: " << done;
	}
}
