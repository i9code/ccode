
#include <iostream>
#include <deque>

static void dequePart() {
	// deque 是c++98中引入的动态数组（dynamic array）
	// namespace std {
	// template<typename T, typename Allocator = allocator<T>>
	// class deque;
	// }
	// 特点随机访问元素，末端和头部添加删除元素效率高。中间删除和添加元素效率低，
	// 元素的访问和迭代比vector要慢，迭代器不能是普通的指针
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
	// 和vector不同，deque不提供以下的函数
	//d.capacity();
	//d.reserve(100);
	d.shrink_to_fit();
	// operator == != < > <= >=

	// 赋值
	b = g;
	b.assign(3, 1.0f);
	b.assign(g.begin(), g.end());
	b.assign({ 1.0f, 2.0f, 3.0f });

	// 交换
	b.swap(a);
	std::swap(a, b);

	// 元素访问
	b[0];
	b.at(0);
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
	 b.shrink_to_fit(); // 为什么没有注意

	// 异常
	// （1） push_back push_front
	// (2) 元素 move/copy 没有异常的话， insert emplace emplace_back push_back
	// (3) pop_back
	// (4) 元素 move/copy 没有异常的话，erase
	// (5) swap clear

}

void main() {
	dequePart();
}



