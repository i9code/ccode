#include <cassert>
#include <memory>
#include <iostream>
 // auto_ptr
 // shared_ptr
 // enable_shared_from_this // CRTP
 // weak_ptr
 // unique_ptr
void sharedPtrNotice();
class Parent;
typedef std::shared_ptr<Parent> ParentPtr;
typedef std::weak_ptr<Parent> WeakParentPtr;
class Child : public std::enable_shared_from_this<Child> {
public:
	WeakParentPtr father;
	~Child();
	Child();
	void checkRelation();
};
typedef std::shared_ptr<Child> ChildPtr;
typedef std::weak_ptr<Child> WeakChildPtr;

class Parent : public std::enable_shared_from_this<Parent> {
public:
	WeakChildPtr son;
	~Parent();
	Parent();
	void checkRelation();
};

void handleChildAndParentRef(const Parent& p, const Child& c) {
	auto cp = c.father.lock();
	auto pc = p.son.lock();
	if (cp.get() == &p && pc.get() == &c) {
		std::cout << "right relation\n";
	}
	else {
		std::cout << "oop!!!!!\n";
	}
}

void handleChildAndParent(const ParentPtr& p, const ChildPtr& c) {
	auto cp = c->father.lock();
	auto pc = p->son.lock();
	if (cp == p && pc == c) {
		std::cout << "right relation\n";
	}
	else {
		std::cout << "oop!!!!!\n";
	}
}

Child::Child() { std::cout << "hello child\n"; }
Parent::Parent() { std::cout << "hello parent\n"; }
Child::~Child() { std::cout << "bye child\n"; }
Parent::~Parent() { std::cout << "bye parent\n"; }
void Parent::checkRelation() {
	auto ps = son.lock();
	if (ps) {
		// this
		handleChildAndParent(shared_from_this(), ps);
	}
	std::cout << "after call checkRelation\n";
}

void Child::checkRelation() {
	// we call handleChildAndParent
}

void testParentAndChild() {
	Parent pp;
	ParentPtr p(new Parent());
	ChildPtr c(new Child());
	p->son = c; // c.use_count() == 2 and p.use_count() == 1
	c->father = p; // c.use_count() == 2 p.use_count() == 2
	p->checkRelation();
}


static void interfaceOfSharedPtr();
static void sharedPtrWithWeakPtr();
static void uniquePtr();
int main() {
	testParentAndChild();
	//	interfaceOfSharedPtr();
	//	sharedPtrWithWeakPtr();
	//	uniquePtr();
}

class Object {
public:
	Object(int id) : m_id(id) { std::cout << "init obj " << m_id << std::endl; }
	~Object() { std::cout << "bye bye " << m_id << std::endl; }
	int id() const { return m_id; }
private:
	int m_id;
};

typedef std::shared_ptr<Object> ObjectPtr;

void deleterOfObject(Object* obj) {
	if (obj)
		std::cout << "delete obj " << obj->id() << std::endl;
	delete obj;
}

void print(const Object& obj) {

}

void print(ObjectPtr obj) {
	std::cout << "count " << obj.use_count() << " id " << obj->id() << std::endl; // 3
}

void printRef(const ObjectPtr& obj) {
	std::cout << "ref count " << obj.use_count() << " id " << obj->id()
		<< std::endl; // 2
}



void sharedPtrWithWeakPtr() {
	ObjectPtr obj(new Object(1));
	typedef std::weak_ptr<Object> WeakObjectPtr;
	WeakObjectPtr weakObj2;
	WeakObjectPtr weakObj(obj);
	WeakObjectPtr weakObj3(obj);
	std::cout << "obj use count is " << obj.use_count() << std::endl; // 1
	{
		auto p = weakObj.lock(); // auto === ObjectPtr
		if (p) {
			std::cout << p.unique() << std::endl; // false p.use_count() >= 2
			// do what you want to do
		}
		else {

		}
	}

	obj.reset(new Object(2));
	{

		auto p = weakObj.lock();
		if (p) {
			assert(false);
			// do what you want to do
		}
		else {

		}
	}

	weakObj = obj;
	if (weakObj.expired()) {

	}
	else {

	}
}

typedef std::unique_ptr<Object> UniqueObjectPtr;

void print(const UniqueObjectPtr& obj) {}
void transfer(UniqueObjectPtr obj) {
	std::cout << obj->id() << std::endl;
}

void uniquePtr() {
	UniqueObjectPtr obj(new Object(1));
	auto p = obj.get(); // operator bool
	if (p) {

	}
	// better
	if (obj) {

	}

	// operator -> *
	std::cout << p->id() << obj->id() << (*obj).id() << std::endl;
	print(obj);
	p = obj.release();
	delete p;

	obj.reset();
	obj.reset(new Object(2)); // obj.reset();
	// UniqueObjectPtr(const UniqueObjectPtr&) = delete
	// UniqueObjectPtr(UniqueObjectPtr&&) = default
	transfer(std::move(obj));

	assert(obj == nullptr);
	//std::cout << obj->id() << std::endl;

	obj.reset(new Object(4));
	ObjectPtr sharedObj(std::move(obj));
	assert(obj == nullptr);
	// shared_ptr weak_ptr enable_shared_from_this
	// unique_ptr
}

void sharedPtrNotice() {
	// ǰ�᣺���Բ�Ҫ�Լ��ֶ��Ĺ�����Դ
	int* a = new int(10);
	delete a;
	int* b = (int*)malloc(sizeof(int));

	if (b) {
		free(b);
	}


	//һ�����ָ�벻Ҫ������shared_ptr����, unique_ptr
	auto pObj = new Object(1);
	ObjectPtr obj(pObj);
	ObjectPtr obj2(pObj);

	// ��weak_ptr����ѭ������,parent �� child
	// ����Ҫ������ڲ��ӿ��У������Ҫ��this��Ϊ����ָ����ʹ�õĻ�����Ҫ
	// �ø���������enable_shared_from_this

	// ʹ��shared_ptr��Ϊ�����Ľӿڣ�����п�����const shared_ptr&����ʽ
	// ���߳�ģʽ��ʹ��shared_ptr��Ҫע���������Ƿŵ����̱߳������������

	// shared_ptr weak_ptr ����ָ����ȣ����ܶ࣬����Ч���ϻ���Ӱ�졣��������
	// ���߳�ģʽ��
	ObjectPtr obj3(new Object(2));
	ObjectPtr obj4 = obj3;

	ObjectPtr obj5 = std::make_shared<Object>(3);
	// ObjectPtr obj5(new Object(3));

// enable_shared_from_this  shared_from_this()�͹�����������һ����ĳЩ������ǲ��ܹ�ʹ�õ�

// ĳЩ����£�������ڴ治�ή�����⡣������ʹ��weak_ptr������ѭ�����õ�����

// ����п��ܣ�����ʹ�����ʵ��������򲻵���ʹ��std::unique_ptr,
// �򲻵���ʹ��std::shared_ptr
	Object obj6(6);

}


void interfaceOfSharedPtr() {
	ObjectPtr null;
	std::cout << "ref count is " << null.use_count() << std::endl; // 0
	ObjectPtr obj(new Object(1));
	std::cout << "ref count is " << obj.use_count() << std::endl; // 1
	ObjectPtr obj2(obj);
	std::cout << "ref count is " << obj.use_count() << std::endl; // 2
	std::cout << "ref count obj2 is " << obj2.use_count() << std::endl; // 2
	ObjectPtr obj3 = obj;
	std::cout << "ref count is " << obj.use_count() << std::endl; // 3
	obj2.reset(); // reset means what?
	obj2 = nullptr;
	std::cout << "ref count is " << obj.use_count() << std::endl; // 2

	ObjectPtr obj4;
	obj3.swap(obj4);
	std::swap(obj3, obj4);
	std::cout << "ref count is " << obj.use_count() << std::endl; // 2

	auto p = obj.get();
	if (p) {
		std::cout << "id is " << p->id() << std::endl;
	}

	if (obj) { // operator bool
		std::cout << "p id is " << obj->id() << std::endl; // operator ->
		std::cout << "ref id is " << (*obj).id() << std::endl; // operator *
	}

	obj4 = nullptr;
	//if(obj.unique())
	//if(obj.use_count() == 1)
	std::cout << "only one hold ptr " << obj.unique() << std::endl;
	print(obj);
	print(*obj);
	std::cout << "ref count is " << obj.use_count() << std::endl; // 2
	printRef(obj);
}

void useDeleter() {
	ObjectPtr obj(new Object(2), deleterOfObject);
	ObjectPtr obj2 = obj;
}

