#include "iostream"
#include "thread"

// Ϊʲô�ö��߳�������������
// ��1����ǰ�Ļ����ĵ��˺����Ѿ�������
// process(linux fork windows CreateProcess, signal) thread (POSIX threads)
// ��2�����߳�ӵ�����������
// ��3��API������죬����ϵͳ�ͱ�׼�ⶼ�Ѿ�֧���˶��߳�
// Ҫ������IO�ͼ����ҵ�ƽ���
// ���̵߳ı�̣�mutilthread goto
// ���ܵ����������
// ��1��������
// ��2������
// ��3����������������ɵ����⣻
// ��4����Ч��

// c++11�����Ǵ������¸���
// ��1���߽׽ӿڣ�(async, future)

// ��2���ͽ׽ӿڣ�(thread, mutex)
//
//

void helloWorld() {
	std::cout << "hello world" << std::endl;
}
int main() {
	// ����һ���߳�
	std::thread t(helloWorld);
	std::cout << "hello main thread" << std::endl;
	// �̵߳��ս�
	t.join();
}