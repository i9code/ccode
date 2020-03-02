#include "iostream"
#include "array"

template<typename C>
void checkSize(C& c) {
	if (c.size() > 3) {
		c[3] = 10;
	}
	//c.at(3) = 10;
}



void arrayPart() {
	// array ʵ�����Ƕ�c/c++������ԭ��������еķ�װ
	// namespace std {
	// template<typename T, size_t N>
	// class array;
	// }
	// �ص㣺�ڴ������ջ��stack�������Բ������·��䣬�������Ԫ��
	int abc[100];
	std::array<int, 100> a;
	std::array<int, 100> b = {};
	// �������ߵ�����

	std::array<int, 5> obj = { 1,2,3,4,5 };
	std::array<int, 5> obj2 = { 1,0 };
	// �ӿ�
	a.empty(); // nerver be true if size > 0
	a.size();
	a.max_size();
	// operator == < != > <= >=
	auto aa = a;

	aa.swap(a);
	swap(aa, a);
	// ����Ԫ��
	a[1];
	a.at(1);
	a.front();
	a.back();
	checkSize(a);

	// ���������
	a.begin();
	a.end();
	a.cbegin();
	a.cend();
	a.rbegin();
	a.rend();
	a.crbegin();
	a.crend();

	// ��C�Ľӿڻ���
	std::array<char, 100> carr;
	strcpy_s(&carr[0],strlen("hello world\n")+1, "hello world\n"); // more better use carr.data();
	printf("%s", &carr[0]); // more better use carr.data()
	// ������÷�
	printf("%s", carr.begin());

	// ����ĵط�
	auto info = std::get<1>(carr);
	carr.fill(0);

	// �쳣exception
	// c.at(pos);
	// copy move swap
}

void main() {
	arrayPart();
}