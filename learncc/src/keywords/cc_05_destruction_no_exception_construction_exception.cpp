// construction

#include "iostream"
#include "fstream"
#include "vector"
#include "string"

using namespace std;

class A {
public:
	A() {  }
	~A() {
		cout << "aaaaa" << endl;
	}
};

class B :public A{
public:
	B() {};
	~B() {
		cout << "bbbb" << endl;
	}
private:
	string m_value;
};

class EvilB :public B {
public:
	~EvilB() { // 默认的析构函数都是 noexcept
		throw string("error");  // 当抛出异常的时候会崩溃直接调用 teminate()  这样会导致内存泄漏   c++中不能在同一个代码块中处理两个异常
	}
};

class GoodB :public B {
public:
	GoodB(const char* f) :m_file(f, ios_base::in) { 
		if (!m_file.is_open())throw string("could not open file"); // 异常尽量在构造函数中抛出
		string line;
		while (getline(m_file, line)) {
			m_info.push_back(line);
		}
	}
private:
	bool isOk()const {
		return !m_info.empty();
	}
private:
	fstream m_file;
	vector<string> m_info;
};

void testGood(const char * filename) {
	int i = 0;
	GoodB b(filename);
	GoodB c(filename);
}
void test() {
	try{
		EvilB b;
	}
	catch (string  const& e){
		cout << "catch you evil " << e << endl;
	}
}

int main() {
	//test();
	testGood("xxxxxxx");
}



