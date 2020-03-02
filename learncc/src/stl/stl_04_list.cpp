#include <iostream>
#include <list>
static void listPart() {

	// list ��c++98�������˫���У�doubley linked list��
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class list;
	// }
	// �ص㲻֧���������Ԫ�أ�����ͷ����β��Ԫ���ٶȿ�
	// �κ�λ�ò���ɾ��Ԫ�ض��ܿ죬����ʱ�������
	// �����ɾ��������ɵ�����ʧЧ
	// �����쳣֧�ֵĺã������쳣����list���ԣ�Ҫ���ɹ���Ҫ��ʲôӰ��û��
	using Group = std::list<float>;

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
	// ��vector��ͬ��list���ṩ���µĺ���
	//d.capacity();
	//d.reserve(100);
	//d.shrink_to_fit();
	// operator == != < > <= >=

	// ��ֵ
	b = g;
	b.assign(3, 1.0f);
	b.assign(g.begin(), g.end());
	b.assign({ 1.0f, 2.0f, 3.0f });

	// ����
	b.swap(a);
	std::swap(a, b);

	// Ԫ�ط���,�����������
	//b[0];
	//b.at(0);
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
	auto iterBegin = a.begin();
	//assert(a.size() >= 10);
	//for(int i = 0;i < 5; ++i) ++iterBegin;
	//std::advance(iterBegin, 4);
	//auto iter5 = std::next(iterBegin, 4);


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
	// �㷨
	b.remove(1.0f);
	b.remove_if([](auto v) { return v > 100.0f; });
	b.reverse(); // 1 2 3 4 -> 4 3 2 1
	//std::sort(a.begin(), a.end());
	b.sort(); // <
	g.sort(); //
	b.merge(g);
	c.unique(); // 1 1 2 2 1 1 3 4 -> 1 2 1 3 4
	c.splice(c.begin(), b);
}


void main() {
	listPart();
}