#include <cassert>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include "map"


static void unorderedMapPart();

static void stlAlgorithm();
static void arrayPart();

int main(int /* argc  */, char** /* argv */) {
	// STL是一个框架，将数据结构和算法进一步抽象。
	// 容器、迭代器、算法
	// Container
	// 1序列式容器 array/vector/deque/list/forward_list 数组 或者 指针 list
	// 2关联类容器 set/map/multiset/multimap 二叉树 红黑树 lgn
	// hash_map/hash_set
	// 3无顺序容器 unordered_map/unordered_set/unordered_multimap/unordered_multiset
	// hash table
	//
	// stack
	// queue
	// priority_queue
	// string
	// bitset bool sizeof(bool) == 1
	//
	// regex rand
	// thread async future time
	//
	//
	//
	//
	// stl 容器的基本知识（常识部分)
	// 容器内元素的条件
	// （1）必须可以复制(copy)或者搬移(move) （隐含的条件是在拷贝和搬移的过程中应该没有副作用）

	//struct BadStruct {
	//	BadStruct() : m_id(g_id++) {}
	//	~BadStruct() { std::cout << "bye\n"; }
	//	// operator =
	//	int m_id;
	//	static int g_id;
	//};
	//BadStruct::g_id = 1;
	// bool char int double pointer
	// reference, function
	// (2) 元素必须可以被赋值操作来复制或者搬移 （因为容器和算法对复写的要求）
	// =
	// （3）元素可以被销毁
	// 针对不同的容器还有特殊的要求
	// 对于序列式容器，元素必须有默认的构造函数
	// 对于某些操作，元素需要定义 == std::find
	// 对于关联式容器，排序准则默认的是 < > std::less
	// 无顺序容器，必须要提供一个hash函数 ==
	// stl容器里面存的是元素的值而不是引用
	// 那么我要问的是，到底我们的容器里面应该存的东西是什么呢？
	// class Player, Monster
	//

	//
	// STL对于错误是怎么处理
	// 我们要记住的是STL的设计原则是效率优先，安全为次
	// 异常？STL自己却不怎么采用。


	// 小小注意
	// set 并不会比 map 更省空间
	// 没有意外的情况下，优先考虑vector (why?)
	// log(n)的查找 和 近似 log(1) 的查找
	// 迭代器失效

	// stl 算法部分 常识部分
	//stlAlgorithm();
}


static void printInfoVersionOne();
static void sortPart();

static void stlAlgorithm() {
	// sort stable_sort
	// remove
	// find find_if
	// count count_if
	// copy copy_if (c++11)
	// swap (非常非常重要的一个函数，意想不到的重要)
	// random_shuffle shuffle
	// .......等等，请自行google
	// stl本身给我们提供了非常非常多的算法，可以替我们解决很多基本的问题，
	// 我们没有必要重新造轮子
	//
	// 一个矫情的例子
	printInfoVersionOne();

	sortPart();
}

static void printInfoVersionOne() {
	using DataStruct = std::vector<std::map<int, std::vector<int>>>;
	DataStruct complicateData;
	// fill data
	// 遍历所有的数据
	for (auto iterOne = complicateData.begin(); iterOne != complicateData.end();
		++iterOne) {
		// do lot of things
		// ba ba ba
		// ba ba ba
		for (auto iterTwo = (*iterOne).begin(); iterTwo != (*iterOne).end();
			++iterTwo) {
			// do lot of things
			// ba ba ba
			// ba ba ba
			for (auto iterThree = (*iterTwo).second.begin();
				iterThree != (*iterTwo).second.end(); ++iterThree) {

				// do lot of things
				// ba ba ba
				// ba ba ba
				// final we get the data
			}
		}
	}
}

static void sortPart() {
	std::vector<int> objects(1000);
	std::generate(std::begin(objects), std::end(objects),
		[]() { return rand(); });
	std::sort(std::begin(objects), std::end(objects));
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	auto engine = std::default_random_engine(seed);
	std::shuffle(std::begin(objects), std::end(objects),
		engine);
	auto objects2{ objects };
	std::stable_sort(std::begin(objects), std::end(objects));
	std::shuffle(std::begin(objects), std::end(objects),
		engine);
}


























template <typename T>
void containerAllInterface(T& a, T& b) {
	T c;
	T d(a); // copy
	T e = a; // copy
	T f(std::move(a)); // move
	auto iterB = b.begin();
	auto iterE = b.end();
	T g(iterB, iterE); // copy
	b.size(); // std::forward_list
	b.empty(); // return b.size() == 0;
	b.max_size();
	if (b == c) { // ==

	}
	if (b != d) { // !(b == d)

	}
	if (b < e) { // unordered_set unordered_map
		// b <= e
		// b > e
		// b >= e

	}
	e = b;
	e = std::move(b);
	e.swap(g); // std::array 线性的， O1
	swap(e, g); // 01 除了 std::array

	e.cbegin(); // -> const_iterator
	auto ea = e.cbegin();
	auto eea = e.begin();
	*eea; // -> ???? const &, &
	*ea; // -> const &

	e.cend();
	e.clear(); // not forward_list, but std::array
}

void test() {
	std::vector<int> a;
	std::vector<int> b;
	// ?? array forward_list unordered_map unordered_set
	containerAllInterface(a, b);
}


