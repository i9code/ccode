#include "iostream"
#include "vector"
#include "cstring"

static void vectorPart() {
	// vector ��c++98������Ķ�̬���飨dynamic array��
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class vector;
	// }
	// �ص��������Ԫ�أ�ĩ�����ɾ��Ԫ��Ч�ʸߡ�ǰ�˺��м�ɾ�������Ԫ��Ч�ʵͣ�
	// ���ڵ�ǰ������С�������Ĺ�ϵ
	using Group = std::vector<float>;
	//typedef std::vector<float> Group;

	Group a;
	Group b = a;
	Group c(a);
	Group d(10);
	Group e(10, 1.0f);
	Group f(e.begin(), e.end());
	Group g({ 1.0f, 2.0f, 3.0f });
	Group h = { 1.0f, 3.0f, 4.0f }; // initialize list

	d.empty();
	d.size();
	d.max_size();
	d.capacity();
	d.reserve(100);
	d.shrink_to_fit(); // c++11
	// operator == != < > <= >=

	// ��ֵ
	b = g;
	b.assign(3, 1.0f); // { 1.0f, 1.0f, 1.0f}
	b.assign(g.begin(), g.end()); // b = g
	b.assign({ 1.0f, 2.0f, 3.0f }); //

	// ����
	b.swap(a); // vector
	std::swap(a, b);

	// Ԫ�ط���
	b[0];
	b.at(0); // std::out_of_range
	if (b.empty()) {
		b.front(); // undefined
		b.back(); // undefined
	}
	b.front();
	b.back();

	// ���������
	a.begin();
	a.end();
	a.cbegin();
	a.cend();
	a.rbegin();
	a.rend();
	a.crbegin();
	a.crend();

	a.pop_back(); // maybe wrong, undefined
	// single thread
	// multithread maybe wrong
	if (!a.empty()) a.pop_back();

	//
	b.erase(b.begin());
	b.erase(b.begin(), b.end());

	b.push_back(10.0f);
	b.pop_back();
	auto iter = b.insert(b.end(), 100.0f);
	iter = b.insert(b.end(), 10, -10.0f);
	b.insert(b.end(), h.begin(), h.end());
	b.emplace(b.end(), 10.0f); // c++11
	b.emplace_back(10.0f); // c++11 && move copy&&
	// b.size() == 100
	b.resize(10);
	// b.size() == 10
	b.resize(100, 1.0f);
	b.resize(100000);
	b.clear(); // notice
	b.shrink_to_fit(); // c++11


	// ��C�Ľӿڻ���
	std::vector<char> carr(100, 0);
	strcpy_s(&carr[0],strlen("hello world\n")+1, "hello world\n"); // more better use carr.data();
	printf("%s", &carr[0]); // more better use carr.data()
	// ������÷�
	printf("%s", carr.begin());

	// �쳣
	// ��1�� push_back
	// (2) Ԫ�� move/copy û���쳣�Ļ��� insert emplace emplace_back push_back
	// (3) pop_back
	// (4) Ԫ�� move/copy û���쳣�Ļ���erase
	// (5) swap clear

	// ����
	// std::vector<bool> never use it


}

int main() {
	vectorPart();
}
