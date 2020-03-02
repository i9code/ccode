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
	~EvilB() { // Ĭ�ϵ������������� noexcept
		throw string("error");  // ���׳��쳣��ʱ������ֱ�ӵ��� teminate()  �����ᵼ���ڴ�й©   c++�в�����ͬһ��������д��������쳣
	}
};

class GoodB :public B {
public:
	GoodB(const char* f) :m_file(f, ios_base::in) { 
		if (!m_file.is_open())throw string("could not open file"); // �쳣�����ڹ��캯�����׳�
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



