#include "iostream"
#include "cstdlib"
#include "string.h"

using namespace std;

class RuleOfThree {
public:
	RuleOfThree(const char* arg) :cstring(new char[strlen(arg) + 1]) {
		strcpy_s(cstring, strlen(arg) + 1, arg);
	}
	~RuleOfThree() {
		delete[] cstring; 
	}
	RuleOfThree(const RuleOfThree& other) {
		cstring = new char[strlen(other.cstring) + 1];
		strcpy_s(cstring, strlen(other.cstring) + 1, other.cstring);
	}
	RuleOfThree& operator=(const RuleOfThree& other) {
		char* tmp_cstring = new char[strlen(other.cstring) + 1];
		strcpy_s(tmp_cstring, strlen(other.cstring) + 1, other.cstring);
		delete[]cstring;
		cstring = tmp_cstring;
		return *this;
	}
private:
	RuleOfThree() = delete; // c11 ɾ��Ĭ�ϵ� 
private:
	char* cstring;
};


// ��ֵ����ȡ��ַ����ֵ����ȡ��ַ


int main() {
	int a = 1; // aΪ��ֵ��1Ϊ��ֵ
	int& zd = a;
	int&& ra = 1;
	RuleOfThree a = RuleOfThree("xxx");
	//cout << a << endl;
	return 0;
}