#include "iostream"
#include "vector"

using namespace std;

class RuleOfFive {
public:
	RuleOfFive(const RuleOfFive &rhs):m_value(new int(*(rhs.m_value))){
	}
	RuleOfFive():m_value(new int(0)) {

	}
	// 右值拷贝构造函数
	RuleOfFive(RuleOfFive&& rhs) {
		delete m_value;
		m_value = rhs.m_value;
		rhs.m_value = nullptr;
	}

	RuleOfFive& operator=(const RuleOfFive& rhs) {
		*m_value = *(rhs.m_value);
		return *this;
	}
	RuleOfFive& operator=( RuleOfFive&& rhs) {
		if (&rhs== this)
			return *this;
		delete m_value;
		m_value = rhs.m_value;
		rhs.m_value = nullptr;
		return *this;
	}
	~RuleOfFive() {
		delete m_value;
	}
private:
	int* m_value;
};

int main() {
	vector<int> a;		// a 左值
	for (int i = 0; i < 10; ++i)
		a.push_back(i);
	RuleOfFive b ; 
	RuleOfFive c = b; //c左值

	// d右值  // 如果重载了右值拷贝构造函数，则调用右值拷贝构造函数，之后再访问m_value则发生异常， 
	// 如果没有重载右值拷贝构造，则调用拷贝构造函数，此时，m_value能够正常访问
	auto d = std::move(b); 
	getchar();
	return 0;
}