// 封装 继承 多态
#include "iostream"
#include "map"

// 在构造函数，和析构函数中调用虚函数实际调用的是类本身的函数
class Event;
class Base {
public:
	Base(int _id):m_id(_id){}
	virtual ~Base() {}
	virtual void act(Event const&) = 0;
	virtual void print() const = 0;
	int id() const { return m_id; }

private:
	int m_id;
};

class Drived : public Base {
public:
	Drived(int _id);
	~Drived();
	void act(Event const&);
	void print()const;
};

class Grouped :public Base {
public:
	Grouped(int id);
	~Grouped();
	void act(Event const&);
	void print()const;
	void addBase(Base* b);
	void removeBase(int id);
private:
	map<int, Base*> m_info;
};