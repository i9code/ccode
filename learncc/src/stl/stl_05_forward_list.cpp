#include <iostream>
#include <forward_list>
static void forwardListPart() {

	// forward_list ��c++11������ĵ����У�singly linked list��
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class forward_list;
	// }
	// �ص㲻֧���������Ԫ�أ�����ͷ��Ԫ���ٶȿ�
	// "forward_list ���Լ���д��c-style singly linked list��ȣ�
	// û���κ�ʱ��Ϳռ��ϵĶ��⿪�����κ�������������Ŀ��ִ������Ƿ�����������"
	// �κ�λ�ò���ɾ��Ԫ�ض��ܿ죬����ʱ�������
	// �����ɾ��������ɵ�����ʧЧ
	// �����쳣֧�ֵĺã������쳣����forward_list���ԣ�Ҫ���ɹ���Ҫ��ʲôӰ��û��
	using Group = std::forward_list<float>;

	Group a;
	Group b = a;
	Group c(a);
	Group d(10);
	Group e(10, 1.0f);
	Group f(e.begin(), e.end());
	Group g({ 1.0f, 2.0f, 3.0f });
	Group h = { 1.0f, 3.0f, 4.0f };

	d.empty();
	//d.size();
	d.max_size();
	// ��vector��ͬ��forward_list���ṩ���µĺ���
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
	//b.back();

	// ���������
	a.begin();
	a.end();
	a.cbegin();
	a.cend();
	//auto iter = a.before_begin();
	//*iter; // undefined
	a.before_begin();
	c.cbefore_begin();
	//	a.rbegin();
	//	a.rend();
	//	a.crbegin();
	//	a.crend();
	auto iterBegin = a.begin();


	//a.pop_back(); // maybe wrong
	//if(!a.empty()) a.pop_back();

	// auto iter = b.erase(b.begin());
	b.erase_after(b.before_begin()); // return void
	b.erase_after(b.before_begin(), b.end()); // return void

	//b.push_back(10.0f);
	//b.pop_back();
	b.push_front(1.2f);
	b.emplace_front(1.3f);
	auto iter = b.insert_after(b.before_begin(), 100.0f);
	iter = b.insert_after(b.before_begin(), 10, -10.0f);
	b.insert_after(b.before_begin(), h.begin(), h.end());
	//b.emplace(b.end(), 10.0f);
	//b.emplace_back(10.0f);
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
	c.splice_after(c.before_begin(), b);

	Group forlist = { 1, 2, 3, 4, 5 };
	auto fiter = forlist.before_begin();
	for (int i = 0; i < 2; ++i) ++fiter;
	forlist.insert_after(fiter, 10);

}

int main() {
	forwardListPart();
}