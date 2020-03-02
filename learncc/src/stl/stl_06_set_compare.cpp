#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include "unordered_map"

class Position {
public:
	int x() const { return m_x; }
	int y() const { return m_y; }
	Position(int _x, int _y) : m_x(_x), m_y(_y) {}
private:
	int m_x = 0;
	int m_y = 0;
};

//// boost
template <class T> inline void hash_combine(std::size_t& seed, const T& v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
namespace std {
	template <> struct hash<Position> {  // 模板类的特化
		size_t operator()(const Position& p) const {
			auto key = hash<int>()(p.x());
			hash_combine(key, p.y());
			return key;
		}
	};
	//template<> struct hash<Position>{ // 模板类的特花
	//	size_t operator()(const Position& p) const {
	//		return p.x() + p.y();
	//	}
	//};
}

class Person {
private:
	int m_age;
	std::string m_name;
public:
	Person(int a, std::string n) : m_age{ a }, m_name{ std::move(n) } {}
	int age() const { return m_age; }
	const std::string& name() const { return m_name; }
	void print() const { std::cout << m_age << " : " << m_name << std::endl; }
	void changeAge(int newAge) { m_age = newAge; }
	bool operator == (Person const& rhs) const {
		return m_age == rhs.m_age && m_name == rhs.m_name;
	}
};

void testUnorderedMap() {
	using Group = std::unordered_map<Position, Person>;
	Group group;
}

struct CompareAge {
	template<typename T>
	bool operator()(const T& t1, const T& t2) const {
		return t1.age() < t2.age();
	}
};

struct CompareName {
	template<typename T>
	bool operator()(const T& t1, const T& t2) const {
		return t1.name() < t2.name();
	}
};

int main() {
	//testUnorderedMap();
	Person a(14, "Alice");
	Person b(25, "Bob");
	Person c(17, "Camily");

	// first we want to put into a set for their age
	// second use name
	std::cout << "order of age:\n";
	using Group = std::set<Person, CompareAge>;
	Group g;
	g.insert(a);
	g.insert(b);
	g.insert(c);
	for (auto& v : g) {
		v.print();
	}
	std::cout << "\n";
	auto ageIter = g.begin();
	//(*iter).changeAge(30);

	std::cout << "order of name:\n";
	using Group2 = std::set<Person, CompareName>;
	Group2 g2;
	g2.insert(a);
	g2.insert(b);
	g2.insert(c);
	for (auto& v : g2) {
		v.print();
	}

	auto iter = g2.find(a);
	if (iter == g2.end()) {

	}
	else {
		// finded
	}

	Person aa(25, "Alice");
	iter = g2.find(aa);
	if (iter == g2.end()) {
		std::cout << "not find aa\n";
	}
	else {
		std::cout << "finded it\n";
	}

	// ==
	iter = std::find(g2.begin(), g2.end(), aa);
	if (iter == g2.end()) {
		std::cout << "not find aa\n";
	}
	else {
		std::cout << "finded it\n";
	}

}