#include "iostream"
#include "map"
#include "unordered_map"
#include "string"


template <typename Map>
typename Map::mapped_type get_default() {
	const Map& map, const typename Map::key_type& key;
	const typename Map::mapped_type& dflt = typename Map::mapped_type()){
	auto pos = map.find(key);
	return (pos != map.end() ? pos->second : dflt);
}
}

static void mapPart() {
	// map multimap 是c++98中引入的二叉树数据结构
	// namespace std {
	// template<typename Key ,typename T, typename Compare = less<T>, typename Allocator = allocator<pair<const Key,T>>
	// class map;
	// template<typename Key,typename T, typename Compare = less<T>, typename Allocator = allocator<pair<const Key,T>>
	// class multimap;
	// }
	// 特点自动将元素排序
	// 插入和删除查找O(logn), 1000, 20
	// 必须元素必须支持严格的弱顺序
	// （1） x < y == true, y < x == false
	// (2) x < y == true, y < z == true, x < z == true
	// (3) x < x === false
	// (4) a == b, b == c, c == a
	// 不能改变元素的值
	// c++ 11 and above
	// map 默认的是用的std::less
	using Group = std::map<int, string>;
	Group sa;
	Group b = sa;
	Group c(sa);
	Group d(sa.begin(), sa.end());
	Group g({ {1,"a"},{2,"b"},{3,"b"} });
	d.empty();
	d.size();
	d.max_size();
	auto keycomp = c.key_comp();
	auto valuecomp = c.value_comp();

	// 赋值
	b = g;
	// 交换
	b.swap(a);
	std::swap(a, b);
	// 迭代器相关
	sa.begin();
	sa.end();
	sa.cbegin();
	sa.cend();
	sa.rbegin();
	sa.rend();
	sa.crbegin();
	sa.crend();
	auto iterBegin = sa.begin();

	// 算法函数
	// map count 0 ,1
	// multimap count >= 0
	auto num = sa.count(1);
	auto findIter = sa.find(1);
	if (findIter == sa.end()) {

	}else {
		std::pair<int, std::string>obj;
		obj = *findIter; // 隐藏了多做一个拷贝构造，效率较低
		*findIter; // std::pair<const int ,std::string>&
	}
	auto state = b.insert(std::make_pair(100, "good")); // state is a pair<Interator,bool>
	auto  insertIter b.insert(c.begin(), c.end());
	b.emplace(std::make_pair(10,"has it"));
	b.emplace(11, std::make_pair("again")); // b.emplace(std::pair<const int,std::string>(10,std::string("agin"));
	b.emplace(12, "thrid");// b.emplace(std::pair<const int,std::string>(10,"agin");  // 效率更高
	
	
	auto& info = b[10];

	b[13]; //=》 auot iter = b.insert(std::make_pair(13,std::string())).first; return (*iter).second;

	std::cout << std::endl;

	auto info = get_default(b, 10);
}


static void unorderedMapPart() {
	// c++ 11 and above
	// why not name hash_map? 为了保证代码的兼容
	using Group = std::unordered_map<int, int>;
	Group a;
	auto ok = a.insert(std::make_pair(1, 1)).second; // 注意返回值
	a[2] = 2; // 使用 [] 是有要求的
	a.size();
	auto isEmpty = a.empty();
	for (int i = 0; i < 10; ++i) a.emplace(i, i); // 注意emplace本身是有返回值的

	// 通过迭代器
	for (auto iter = a.begin(), iterEnd = a.end(); iter != iterEnd; ++iter) {
		// 注意迭代器访问的输出是无序的
		std::cout << "key: " << (*iter).first << " value: " << iter->second << " ";
	}
	std::cout << std::endl;

	// 通过key索引
	for (int i = 0; i < a.size(); ++i) {
		std::cout << a.at(i) << " "; // 注意a[i]的区别
	}

	a.rehash(10); // ? 干了什么事情呢？
	std::cout << std::endl;
}


static void unorderedSetAndMapPart() {

	// unordered_multiset unordered_set
	// unordered_map unordered_multimap是c++11中以hash table为基础的，内部元素没有明确的顺序的容器
	// namespace std {
  // template<typename Key, typename T, typename Hash = hash<Key>, typename
  // EqPred = equal_to<Key>,  typename Allocator =
  // allocator<pair<const Key, T>>
  // class unorderd_map;
  // template<typename Key, typename T, typename Hash = hash<Key>, typename
  // EqPred = equal_to<T>,typename Allocator =
  // allocator<pair<const Key, T>>
  // class multimap;
  // template<typename T, typename Hash = hash<T>, typename
  // EqPred = equal_to<T>,  typename Allocator =
  // allocator<T>>
  // class unorderd_set;
  // template<typename T, typename Hash = hash<T>, typename
  // EqPred = equal_to<T>,  typename Allocator =
  // allocator<T>>
  // class unorderd_multiset;
  // }
  // 插入和删除查找O(1)
  // 不能改变Key元素的值
  //
	using Group = std::unordered_map<int, std::string>;
	Group a;
	Group b = a;
	Group c(a);
	Group d(c.begin(), c.end());
	Group g({ {1, "a"}, {2,"test"}, {3, "test"} });

	d.empty();
	d.size();
	d.max_size();
	// operator == !=
	//  special
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
	// map count 0 1
	// multimap count >= 0
	auto num = a.count(1);
	auto findIter = a.find(1);
	if (findIter == a.end()) {
		// not finded
	}
	else {
		const std::pair<int, std::string>& obj = *findIter;
		//*findIter; // std::pair<const int, std::string>&
	}
	auto lower = a.lower_bound(1);
	if (lower != a.end()) {
		if (*lower.first == 1) {
			// has 1
		}
	}
	auto high = a.upper_bound(1);
	auto range = a.equal_range(1); // return std::make_pair(a.lower_bound(1), a.upper_bound(1));

	auto eraseIter = b.erase(b.begin());
	eraseIter = b.erase(b.begin(), b.end());

	auto state = b.insert(std::make_pair(100, "good")); // state is a pair<Iterator, bool>
	auto insertIter = b.insert(c.begin(), c.end());
	b.emplace(std::make_pair(10, "has it"));
	b.emplace(11, std::string("again")); // b.emplace( std::pair<const int,std::string>(10, std::string("again"));
	b.emplace(12, "third"); // b.emplace(std::pair<const int, std::string>(10, "third"));
	b.emplace_hint(b.end(), 13, "haha");

	// []
	// auto string
	auto& info = b[10];
	// [] -> auto iter = b.insert(std::make_pair(13, std::string())).first; return (*iter).second;
	b[13];

	// at
	try {
		auto& findInfo = b.at(10); // const
	}
	catch (...) {

	}
	auto findIter = b.find(10);
	if (findIter != std::end(b) /* b.end() */) {
		auto& v = (*findIter).second;
	}
	else {

	}

	auto info = get_default(b, 10);
	if (info.empty()) {

	}
	else {

	}
}




int main() {
	unorderedMapPart();
	mapPart();
}