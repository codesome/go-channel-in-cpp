/*
* Author: @thecodesome (https://github.com/thecodesome)
* Github: https://github.com/thecodesome/go-channel-in-cpp
**/

#ifndef __CHANNEL_HPP__

#define __CHANNEL_HPP__

#include <queue>
#include <mutex>
#include <atomic>
#include <semaphore.h>
using namespace std;

namespace channel {
	const int SUCCESS = 0;
	const int CLOSED = -1;
}

template <class T>
class Channel {

public:
	// Constructor
	Channel();

	/*
	* @Params: (Data to add in channel queue)
	* returns 'false' if channel is closed, else true
	**/
	int add(const T data);

	/*
	* @Params: (bool variable to be set to true if channel is closed else false)
	* returns data from the queue of added elements. 
	* If channel is closed, it returns empty element.
	* Note: Check if the channel is closed from the variable passed. 
	* 		Data returned is junk if channel is closed.
	**/
	T retrieve(bool *closed);

	/*
	* @Params: None
	* Used to close the channel
	**/
	int close();

	/*
	* @Params: None
	* returns the buffer size of channel
	**/
	int buffer_size();

private:
		
	// buffer to store incoming data
	queue<T> buffer;

	// true if channel is closed
	bool chan_closed;

		// to lock operations on buffer queue
	mutex buffer_mutex;

		// to lock operations on chan_queue
	mutex close_mutex;

		// to check if queue is empty 
	sem_t buffer_sem;

		// number of retrieve calls waiting for data
	atomic<int> waiting_length; 

}; /*class Channel*/



template <class T>
Channel<T>::Channel() {
	sem_init(&buffer_sem, 0, 0);
	waiting_length = 0;
	chan_closed = false;
}

template <class T>
int Channel<T>::add(const T data) {

	close_mutex.lock();
	if(!chan_closed) {
		buffer_mutex.lock();
		buffer.push(data);
		buffer_mutex.unlock();
		close_mutex.unlock();
		
		sem_post(&buffer_sem);
		return channel::SUCCESS;
	} else {
		close_mutex.unlock();
		return channel::CLOSED;
	}

}

template <class T>
T Channel<T>::retrieve(bool *closed) {

	close_mutex.lock();
	if(!chan_closed) {
		
		++waiting_length;
		close_mutex.unlock();

		sem_wait(&buffer_sem);
		--waiting_length;

	} else if(buffer.size()==0) {
		close_mutex.unlock();
		*closed = true;
		T temp;
		return temp;
	} else {
		close_mutex.unlock();
	}

	buffer_mutex.lock();

	if(buffer.size()>0) {
		T data = buffer.front();
		buffer.pop();
		buffer_mutex.unlock();
		*closed = false;
		return data;
	} else {
		buffer_mutex.unlock();
		*closed = true;
		T temp;
		return temp;
	}

}

template <class T>
int Channel<T>::close() {

	close_mutex.lock();
	if(!chan_closed){
		chan_closed = true;
		int current_waiting = waiting_length;
		while(--current_waiting >= 0) {
			sem_post(&buffer_sem);
		}
		close_mutex.unlock();
		return channel::SUCCESS;
	} else {
		close_mutex.unlock();
		return channel::CLOSED;
	}

}

template <class T>
int Channel<T>::buffer_size() {
	return buffer.size();
}

#endif /*__CHANNEL_HPP__*/