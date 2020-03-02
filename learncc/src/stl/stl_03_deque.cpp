
#include <iostream>
#include <deque>

static void dequePart() {
	// deque ��c++98������Ķ�̬���飨dynamic array��
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class deque;
	// }
	// �ص��������Ԫ�أ�ĩ�˺�ͷ�����ɾ��Ԫ��Ч�ʸߡ��м�ɾ�������Ԫ��Ч�ʵͣ�
	// Ԫ�صķ��ʺ͵�����vectorҪ������������������ͨ��ָ��
	using Group = std::deque<float>;

	Group a;
	Group b = a;
	Group c(a);
	Group d(10);
	Group e(10, 1.0f);
	Group f(e.begin(), e.end());
	Group g({ 1.0f, 2.0f, 3.0f });
	Group h = { 1.0f, 3.0f, 4.0f };

	d.empty();
	d.size();
	d.max_size();
	// ��vector��ͬ��deque���ṩ���µĺ���
	//d.capacity();
	//d.reserve(100);
	d.shrink_to_fit();
	// operator == != < > <= >=

	// ��ֵ
	b = g;
	b.assign(3, 1.0f);
	b.assign(g.begin(), g.end());
	b.assign({ 1.0f, 2.0f, 3.0f });

	// ����
	b.swap(a);
	std::swap(a, b);

	// Ԫ�ط���
	b[0];
	b.at(0);
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

	a.pop_back(); // maybe wrong
	if (!a.empty()) a.pop_back();

	b.erase(b.begin());
	b.erase(b.begin(), b.end());

	b.push_back(10.0f);
	b.pop_back();
	b.push_front(1.2f);
	b.emplace_front(1.3f);
	auto iter = b.insert(b.end(), 100.0f);
	iter = b.insert(b.end(), 10, -10.0f);
	b.insert(b.end(), h.begin(), h.end());
	b.emplace(b.end(), 10.0f);
	b.emplace_back(10.0f);
	b.resize(10);
	b.resize(100, 1.0f);
	b.clear(); // notice
	 b.shrink_to_fit(); // Ϊʲôû��ע��

	// �쳣
	// ��1�� push_back push_front
	// (2) Ԫ�� move/copy û���쳣�Ļ��� insert emplace emplace_back push_back
	// (3) pop_back
	// (4) Ԫ�� move/copy û���쳣�Ļ���erase
	// (5) swap clear

}

void main() {
	dequePart();
}



