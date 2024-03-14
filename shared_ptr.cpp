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

class SharedPtrA {
public:
	SharedPtrA(const SharedPtrA& other);
	SharedPtrA(A* p = nullptr);
	~SharedPtrA() {
		_delete();
	}
	SharedPtrA& operator=(const SharedPtrA& other);
	A* operator->() const { return _a; }
	A& operator*() const { return (*_a); }
	int use_count() const { return *_use_count; }
	A* get() const { return _a; }
	void reset(A* p = nullptr);
private:
	A* _a = nullptr;
	int* _use_count;
	void _delete() {
		(*_use_count)--;
		if ((*_use_count) < 1) {
			delete _use_count;
			delete _a;
		}
	}
};

SharedPtrA::SharedPtrA(A* p) : _a(p) {
	p != nullptr ? _use_count = new int(1) : _use_count = new int(0);
}

SharedPtrA::SharedPtrA(const SharedPtrA& other) : _a(other._a), _use_count(other._use_count) {
	(*_use_count)++;
}

SharedPtrA& SharedPtrA::operator=(const SharedPtrA& other) {
	if (_a != other._a) {
		_delete();
		_a = other._a;
		_use_count = other._use_count;
		(*_use_count)++;
	}
	return *this;
}

void SharedPtrA::reset(A* p) {
	_delete();
	_a = p;
	_use_count = (new int(1));
}

void errors(int& done, int line) {
	done++;
	std::cout << "error in " << line << std::endl;
}

int main() {
	int n_destructs = 0;
	SharedPtrA a(new A(&n_destructs));
	int done = 0;

	CHECK(a.use_count() == 1);
	CHECK(a.get()->value() == 0);
	CHECK(a->value() == 0);

	SharedPtrA b = a;
	a->change_value();
	CHECK(a->value() == 1);
	CHECK(b->value() == 1);
	CHECK(a.get() == b.get());
	CHECK(a.use_count() == 2);
	CHECK(b.use_count() == 2);

	SharedPtrA c(b);
	(*b).change_value();
	CHECK(c->value() == 2);
	CHECK(a->value() == 2);
	CHECK(a.use_count() == 3);
	CHECK(b.use_count() == 3);
	CHECK(c.use_count() == 3);


	a.reset();
	CHECK(n_destructs == 0);
	a.reset(new A(&n_destructs));
	CHECK(a.get() != c.get());
	CHECK(a.use_count() == 1);
	CHECK(b.use_count() == 2);
	a = c;
	CHECK(n_destructs == 1);
	CHECK(a.use_count() == 3);

	a.reset();
	b.reset();
	c.reset();

	CHECK(n_destructs == 2);
	{
		SharedPtrA(new A(&n_destructs));
	}
	CHECK(n_destructs == 3);


	{
		SharedPtrA a;
		CHECK(a.get() == nullptr);
		CHECK(a.use_count() == 0);
	}
	{
		const SharedPtrA a(new A(&n_destructs));
		CHECK(a.get() != nullptr);
		a->change_value();
		a.get()->change_value();
		CHECK((*a).value() == 2);
		CHECK(a.get()->value() == 2);
		CHECK(a.use_count() == 1);
		//a.reset();  //  should not compile
		SharedPtrA b(a);
		CHECK(b.use_count() == 2);
	}
	CHECK(n_destructs == 4);
	{
		SharedPtrA a(new A(&n_destructs));
		a = a;
		CHECK(a.use_count() == 1);
	}
	CHECK(n_destructs == 5);
	if (done == 0) {
		std::cout << "DONE" << std::endl;
	}
	else {
		std::cout << "Count of errors: " << done;
	}
}
