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
	// STL��һ����ܣ������ݽṹ���㷨��һ������
	// ���������������㷨
	// Container
	// 1����ʽ���� array/vector/deque/list/forward_list ���� ���� ָ�� list
	// 2���������� set/map/multiset/multimap ������ ����� lgn
	// hash_map/hash_set
	// 3��˳������ unordered_map/unordered_set/unordered_multimap/unordered_multiset
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
	// stl �����Ļ���֪ʶ����ʶ����)
	// ������Ԫ�ص�����
	// ��1��������Ը���(copy)���߰���(move) ���������������ڿ����Ͱ��ƵĹ�����Ӧ��û�и����ã�

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
	// (2) Ԫ�ر�����Ա���ֵ���������ƻ��߰��� ����Ϊ�������㷨�Ը�д��Ҫ��
	// =
	// ��3��Ԫ�ؿ��Ա�����
	// ��Բ�ͬ���������������Ҫ��
	// ��������ʽ������Ԫ�ر�����Ĭ�ϵĹ��캯��
	// ����ĳЩ������Ԫ����Ҫ���� == std::find
	// ���ڹ���ʽ����������׼��Ĭ�ϵ��� < > std::less
	// ��˳������������Ҫ�ṩһ��hash���� ==
	// stl������������Ԫ�ص�ֵ����������
	// ��ô��Ҫ�ʵ��ǣ��������ǵ���������Ӧ�ô�Ķ�����ʲô�أ�
	// class Player, Monster
	//

	//
	// STL���ڴ�������ô����
	// ����Ҫ��ס����STL�����ԭ����Ч�����ȣ���ȫΪ��
	// �쳣��STL�Լ�ȴ����ô���á�


	// ССע��
	// set ������� map ��ʡ�ռ�
	// û�����������£����ȿ���vector (why?)
	// log(n)�Ĳ��� �� ���� log(1) �Ĳ���
	// ������ʧЧ

	// stl �㷨���� ��ʶ����
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
	// swap (�ǳ��ǳ���Ҫ��һ�����������벻������Ҫ)
	// random_shuffle shuffle
	// .......�ȵȣ�������google
	// stl����������ṩ�˷ǳ��ǳ�����㷨�����������ǽ���ܶ���������⣬
	// ����û�б�Ҫ����������
	//
	// һ�����������
	printInfoVersionOne();

	sortPart();
}

static void printInfoVersionOne() {
	using DataStruct = std::vector<std::map<int, std::vector<int>>>;
	DataStruct complicateData;
	// fill data
	// �������е�����
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
	e.swap(g); // std::array ���Եģ� O1
	swap(e, g); // 01 ���� std::array

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


