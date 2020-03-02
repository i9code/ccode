#include <iostream>
#include <forward_list>
static void forwardListPart() {

	// forward_list 是c++11中引入的单向串列（singly linked list）
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class forward_list;
	// }
	// 特点不支持随机访问元素，访问头部元素速度快
	// "forward_list 和自己手写的c-style singly linked list相比，
	// 没有任何时间和空间上的额外开销。任何性质如果和这个目标抵触，我们放弃该特征。"
	// 任何位置插入删除元素都很快，常量时间内完成
	// 插入和删除不会造成迭代器失效
	// 对于异常支持的好，出现异常对于forward_list而言，要不成功，要不什么影响没有
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
	// 和vector不同，forward_list不提供以下的函数
	//d.capacity();
	//d.reserve(100);
	//d.shrink_to_fit();
	// operator == != < > <= >=

	// 赋值
	b = g;
	b.assign(3, 1.0f);
	b.assign(g.begin(), g.end());
	b.assign({ 1.0f, 2.0f, 3.0f });

	// 交换
	b.swap(a);
	std::swap(a, b);

	// 元素访问,不能随机访问
	//b[0];
	//b.at(0);
	b.front();
	//b.back();

	// 迭代器相关
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
	// 算法
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