#include "iostream"

using namespace std;

// alignas   alignof		size and		bool  constexpr   
// decltype  dynamic_cast	enum explicit
// friend	 nullptr		operator  


// static_assert  // 在编译器就能进行断言，不能在运行期进行断言，  assert在运行期进行断言

typedef unsigned long ulong;
class CTypedefComp {
	typedef map<int, ulong> Group;
	Group a;
	void aa() {
		Group::iterator iter = a.find(10);
		auto iter = a.find(10);// c11
	}
};

int main() {
	//_nullptr();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class COperator {
public:
	COperator() :m_value(new int()) {}
	COperator(const COperator& rhs) :m_value(new int(*(rhs.m_value))) {}
	COperator& operator=(const COperator& rhs) {
		*m_value = *(rhs.m_value);
		return *this;
	}
	~COperator() {}
private:
	int* m_value;
};

void _operator() {
	COperator a;
	COperator b = a; // 如果不对拷贝构造进行重载就会导致m_value 一个地址被多个对象引用，对象被释放，m_value 将会释放多次出现bug
	b = a;// 如果不对赋值函数进行重载就会导致m_value 一个地址被多个对象引用，对象被释放，m_value 将会释放多次出现bug
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// nullptr是一种类型  
template<typename T, typename U>
void func_nullptr(T t, U u) {
	t(u);
}
void null_pointer(int* a) {
	cout << "i'm a pointer\n" << endl;
}

void _nullptr() {
	null_pointer(0);
	null_pointer(NULL);
	null_pointer(nullptr);
	// func_nullptr(null_pointer, 0); // 模板中u推导出来为 int  而null_pointer 需要的为 int*
	// func_nullptr(null_pointer, NULL);
	func_nullptr(null_pointer, nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CFriend {
	int data;
	friend ostream& operator<<(ostream& out, const CFriend& o);
	template<typename T>friend class B; // B 是CFriend 的 friend 但不是 CFriend子类的friend
	template<typename T> friend void f(T) {}
};

ostream& operator<<(ostream& out, const CFriend& o) {

	return out << o.data;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SExplicitB
{
	explicit SExplicitB(int) {}
	explicit SExplicitB(int, int) {}
	explicit  operator int() const {
		return 0;
	}
};

struct SNoExplicitA
{
	SNoExplicitA(int) {}
	SNoExplicitA(int, int) {}
	operator int() const {
		return 0;
	}
};


void _explicit() {
	SNoExplicitA a1 = 1; // 等价与  SNoExplicitA a1 = SNoExplicitA(1)
	// SExplicitB b1 = 1; // error 

	SNoExplicitA a2(2);
	SExplicitB b2(2); // ok

	SNoExplicitA a3{ 4, 5 }; // 等价 SNoExplicitA a3 = SNoExpliciA(4,6); SNoExplicitA{4,5};
	//SExplicitB b3{ 4, 5 }; // error

	int na1 = a1; //ok
	//int nb1 = b2; // error

	int na2 = static_cast<int>(a1);
	int nb2 = static_cast<int>(b2);

	SNoExplicitA a4 = (SNoExplicitA)1;
	SExplicitB b4 = (SExplicitB)1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum EColor { // before c11  red 为全局的会污染用名空间
	red,
	green,
	blue,
};

//  enum class ECColor:short // 最大只有一个char的大小1-127
enum class ECColor
{
	red,
	green,
	blue,
};


void _enum() {
	EColor r = red;
	switch (r)
	{
	case red:
		cout << "i'm red" << endl;
		break;
	case green:
		cout << "i'm green" << endl;
		break;
	case blue:
		cout << "i'm blue" << endl;
		break;
	default:
		cout << "i'm what" << endl;
		break;
	}

	// 推荐使用c11后的写法
	ECColor rr = ECColor::red;
	switch (rr)
	{
	case ECColor::red:
		cout << "i'm red" << endl;
		break;
	case ECColor::green:
		cout << "i'm green" << endl;
		break;
	case ECColor::blue:
		cout << "i'm blue" << endl;
		break;
	default:
		cout << "i'm what" << endl;
		break;
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SBaseDynamic {
	virtual~SBaseDynamic() {}
};

struct SDerivedDynamic :SBaseDynamic {
	virtual void name() {}
};

void _dynamic_cast() {
	SBaseDynamic* b1 = new SBaseDynamic;
	if (SDerivedDynamic* d = dynamic_cast<SDerivedDynamic*>(b1)) {
		cout << "downcast from b1 to d successful" << endl;
		d->name(); // safe to  call;
	}

	SBaseDynamic* b2 = new SDerivedDynamic;
	if (SDerivedDynamic* d = dynamic_cast<SDerivedDynamic*>(b2)) {
		cout << "downcast from b2 to d successful" << endl;
		d->name(); // safe to call
	}

	// dynamic_cast转化引用失败会抛出异常
	// SBaseDynamic bb;
	// SDerivedDynamic& cc = dynamic_cast<SDerivedDynamic&>(bb);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SDecl
{
	double x;
};

template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) { // 后缀返回法，后缀表达式
	return a + b;
}

void _decltype() {
	const SDecl* sd = new SDecl{ 0 };

	auto x = sd->x; // x 类型为double
	auto& cc = x; // cc也为double&

	decltype (sd->x) y;// y类型为double 
	decltype ((sd->x)) z = y; // z的类型为 double&

	auto  ss = add(1.0, 3); // ss 为double 
	auto  si = add(1, 1);// si 为int类型
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _const_cast() {
	const int j = 3;
	const int* cpj = &j;
	// *cpj = 4;  // error 不能为常量赋值
	int* pj = const_cast<int*>(&j); // 等价于 int* pj = const_cast<int*>(cpj)
	cout << *pj << endl;
	*pj = 4;  // undefined behavior!
	cout << *pj << endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fact(int n) {
	return n < 1 ? 1 : (n * fact(n - 1));
}

constexpr int constexpr_fact(int n) {
	return n < 1 ? 1 : (n * constexpr_fact(n - 1));
}

void _constexpr() {
	auto a = fact(4); // 在运行期为a赋值24
	auto b = constexpr_fact(4);  // 如果数据足够多 在编译期直接为b赋值24  // 如果在编译器不能确定，就转到运行期确认
	char group[constexpr_fact(6)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _bool() {
	bool a = true;
	bool b = false;
	*(reinterpret_cast<char*>(&a)) = -1;
	cout << a << "  " << b << endl;
	if (a == true) {
		cout << "i'm ture" << endl;
	}
	else if (a == false) {
		cout << "i'm false" << endl;
	}
	else {
		cout << "i'm what ？？" << endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _and() {
	int a = 3, b = 4;
	// && 等价与 and
	if (a == 3 && b == 4){
		//TODO
	}
	if (a == 3 and b == 4){
		//TODO
	}

	// bitand  <=>  &
	// bitor   <=>  |
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct alignas(8) S {};

//struct alignas(1) U { S s; };  // 这种无效过

struct Foo
{
	int i;
	float f;
	char c;
};

struct Empty {};

struct SSizeSub
{

};

struct alignas (8) Empty64 {};

struct Obj {
	char a;
	int b;
};


// alignas 设置结构体的对齐方式
// alignof  求结构体对齐长度
void _align() {
	cout << "Aligenment   of" << endl;
	cout << "- char        :" << alignof(char) << endl;
	cout << "- pointer     :" << alignof(int*) << endl;
	cout << "- class Foo   :" << alignof(Foo) << endl;
	cout << "- empty class :" << alignof(Empty) << endl;
	cout << "- emp64 class :" << alignof(Empty64) << endl;
	cout << "- alignas(1) L:" << alignof(double) << endl;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SSizeEmpty {};
struct SSizeBase { int a; };
struct SSizeDerived :SSizeBase { int b; };
struct SSizeBit { unsigned bit : 1; };

void _size() {
	SSizeEmpty e;
	SSizeDerived d;
	SSizeBase& b = d;
	SSizeBit bit;
	cout << "Size				   of" << endl;
	cout << "- char					:" << sizeof(char) << endl;
	cout << "- pointer				:" << sizeof(int*) << endl;
	cout << "- struct SSizeEmpty	:" << sizeof(SSizeEmpty) << endl;
	cout << "- struct SSizeBase		:" << sizeof(SSizeBase) << endl;
	cout << "- struct SSizeBit		:" << sizeof(SSizeBit) << endl;
	cout << "- struct e				:" << sizeof(e) << endl;
	cout << "- struct d				:" << sizeof(d) << endl;
	cout << "- struct b				:" << sizeof(b) << endl;
	cout << "- struct bit			:" << sizeof(bit) << endl;
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////