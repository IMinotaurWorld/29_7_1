#include <iostream>
#include <mutex>

using namespace std;

struct Node
{
	Node(int _value) : value(_value), node_mutex(new mutex()){
	}
	int value;
	Node* next;
	std::mutex* node_mutex;
};

class FineGrainedQueue
{
public:
	Node* head;
	std::mutex* queue_mutex;
	
	FineGrainedQueue() : queue_mutex(new mutex()){
	}
	
	void insertIntoMiddle(int value, int pos){
		Node* newnode = new Node(value);
		newnode->node_mutex->lock();
		
		queue_mutex->lock();
		
		Node* prev = head;
		prev->node_mutex->lock();
		queue_mutex->unlock();
		
		int i = 1;
		while(true){
			if(i == pos || prev->next == nullptr){
				newnode->next = prev->next;
				prev->next = newnode;
				prev->node_mutex->unlock();
				newnode->node_mutex->unlock();
				break;
			}
			prev->node_mutex->unlock();
			prev = prev->next;
			prev->node_mutex->lock();	
			i++;
		}
	}
};

int main(int argc, char** argv) {
	FineGrainedQueue queue;
	queue.head = new Node(10);
	queue.head->next = new Node(20);
	queue.head->next->next = new Node(30);
	queue.head->next->next->next = new Node(40);
	queue.head->next->next->next->next = new Node(50);
	queue.head->next->next->next->next->next = nullptr;
	
	Node* outnode = queue.head;
	while (outnode != nullptr){
		cout << outnode->value << " ";
		outnode = outnode->next;
	}
	
	cout << '\n';
	
	queue.insertIntoMiddle(100, 1);
	
	outnode = queue.head;
	while (outnode != nullptr){
		cout << outnode->value << " ";
		outnode = outnode->next;
	}
	
	return 0;
}