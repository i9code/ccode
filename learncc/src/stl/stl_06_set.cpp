#include "iostream"
#include "set"
#include <unordered_set>

static void setPart() {

	// set multiset 是c++98中引入的二叉树数据结构
	// namespace std {
  // template<typename T, typename Compare = less<T>, typename Allocator =
  // allocator<T>>
  // class set;
	//
  // template<typename T, typename Compare = less<T>, typename Allocator =
  // allocator<T>>
  // class multiset;
  // }
  // 特点自动将元素排序
  // 插入和删除查找O(logn), 1000, 20
	// 必须元素必须支持严格的弱顺序
	// （1） x < y == true, y < x == false
	// (2) x < y == true, y < z == true, x < z == true
	// (3) x < x === false
	// (4) a == b, b == c, c == a
	// 不能改变元素的值
	//
	// 辅助的类
	// namespace std {
	// template<typename T1, typename T2>
	// struct pair {
	// T1 first;
	// T2 second;
	// };
	// }
	using Group = std::set<float>;
	Group a;
	Group b = a;
	Group c(a);
	Group d(c.begin(), c.end());
	Group g({ 1.0f, 4.0f, 3.0f });
	// 1.0f
	// 1.0f 4.0f
	// 1.0f 3.0f 4.0f -> 1.0f 4.0f 3.0f
	Group h = { 1.0f, 3.0f, 4.0f };

	d.empty();
	d.size();
	d.max_size();
	// operator == != < > <= >=
	// special
	auto keycomp = c.key_comp();
	auto valuecomp = c.value_comp();

	// 赋值
	b = g;

	// 交换
	b.swap(a);
	std::swap(a, b);


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

	//算法函数
	// set count 0 1
	// multiset count >= 0
	auto num = a.count(1.0f);
	auto findIter = a.find(1.0f);
	if (findIter == a.end()) {
		// not finded
	}
	else {
		*findIter;
	}
	auto lower = a.lower_bound(1.0f);
	if (lower != a.end()) {
		if (*lower == 1.0f) {
			// has 1.0f
		}
	}
	auto high = a.upper_bound(1.0f);
	auto range = a.equal_range(1.0f); // return std::make_pair(a.lower_bound(1.0f), a.upper_bound(1.0f));

	auto eraseIter = b.erase(b.begin());
	eraseIter = b.erase(b.begin(), b.end());

	auto state = b.insert(100.0f); // state is a pair<Iterator, bool>
	//auto insertIter = b.insert(c.begin(), c.end());
	b.emplace(10.0f);
	b.emplace_hint(b.end(), 100.0f);
}

static void unorderedSetPart() {
	// c++ 11 and above
	using Group = std::unordered_set<int>;
	Group a;
	auto ok = a.insert(1).second; // 注意返回值
	a.size();
	auto isEmpty = a.empty();
	for (int i = 0; i < 10; ++i) a.emplace(i); // 注意emplace本身是有返回值的

	// 通过迭代器
	for (auto iter = a.begin(), iterEnd = a.end(); iter != iterEnd; ++iter) {
		// 注意迭代器访问的输出的是有顺序的
		std::cout << " value: " << *iter << " ";
	}
	std::cout << std::endl;
}


int main() {
	setPart();
}