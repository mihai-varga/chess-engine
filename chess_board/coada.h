#ifndef __QUEUE_H_
#define __QUEUE_H_
template<typename T, int N>
class Queue {
	private:
		int head;
		int tail;
		int size;
		T queueArray[N];

	public:
		Queue() {
			size = 0;
			head = tail = 0;
		}

		~Queue() {
		}

		void enqueue(T e) {
			size++;
			queueArray[tail] = e;
			tail = (tail + 1) % N;
		}

		T dequeue() {
			T x;
			x = queueArray[head];

			head = (head + 1) % N;
			size--;
			return x;
			
		}

		T front() {
			T x;
			x = queueArray[head];
			return x;
		}

		bool isEmpty() {
			if (size) return 0;
			else return 1;
		}
};

#endif
