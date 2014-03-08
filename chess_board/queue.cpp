#include "coada.h"
#include <iostream>
#define NMAX 200
int main() {
	Queue<int, NMAX> q;
	q.enqueue(7);
	q.enqueue(8);
	q.enqueue(9);
	std::cout << q.dequeue();
	std::cout << q.front();
	std::cout << q.dequeue();
	return 0;
}
