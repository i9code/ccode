#include "iostream"
#include "vector"

using namespace std;

class RuleOfFive {
public:
	RuleOfFive(const RuleOfFive &rhs):m_value(new int(*(rhs.m_value))){
	}
	RuleOfFive():m_value(new int(0)) {

	}
	// ��ֵ�������캯��
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
	vector<int> a;		// a ��ֵ
	for (int i = 0; i < 10; ++i)
		a.push_back(i);
	RuleOfFive b ; 
	RuleOfFive c = b; //c��ֵ

	// d��ֵ  // �����������ֵ�������캯�����������ֵ�������캯����֮���ٷ���m_value�����쳣�� 
	// ���û��������ֵ�������죬����ÿ������캯������ʱ��m_value�ܹ���������
	auto d = std::move(b); 
	getchar();
	return 0;
}