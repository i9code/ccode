#include "iostream"
#include "vector"
using namespace std;

class RuleOfFive; // ǰ������  ����������

class Parent;
class Child {
public:
	Child(Parent*p):m_parent(p){}  //  
private:
	Parent* m_parent;
};

class Parent {
public:
	bool addChild(Child* clild) {
		m_children.push_back(clild);
	}
private:
	vector<Child*> m_children;
};
