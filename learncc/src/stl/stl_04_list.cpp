#include <iostream>
#include <list>
static void listPart() {

	// list 是c++98中引入的双向串列（doubley linked list）
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class list;
	// }
	// 特点不支持随机访问元素，访问头部和尾部元素速度快
	// 任何位置插入删除元素都很快，常量时间内完成
	// 插入和删除不会造成迭代器失效
	// 对于异常支持的好，出现异常对于list而言，要不成功，要不什么影响没有
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
	// 和vector不同，list不提供以下的函数
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
	b.back();

	// 迭代器相关
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
	// 算法
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