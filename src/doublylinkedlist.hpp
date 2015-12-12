#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include "doublylinkedlistitem.hpp"

template<class T> class DoublyLinkedList {

public:
	DoublyLinkedList() : 
	_front(0x0),
	_back(0x0),
	_size(0) {

	}

	DoublyLinkedListItem<T> *push_back(const T &val) {
		DoublyLinkedListItem<T> *item = 0x0;
		if (_size > 0) {
			item = new DoublyLinkedListItem<T>(_back, 0x0, val);
			_back->next = item;
			_back = item;
		} else {
			item = new DoublyLinkedListItem<T>(0x0, 0x0, val);
			_front = item;
			_back = item;
		}
		_size++;
		return item;
	}

	DoublyLinkedListItem<T> *push_front(const T &val)  {
		DoublyLinkedListItem<T> *item = 0x0;
		if (_size > 0) {
			item = new DoublyLinkedListItem<T>(0x0, _front, val);
			_front->previous = item;
			_front = item;
		} else {
			item = new DoublyLinkedListItem<T>(0x0, 0x0, val);
			_front = item;
			_back = item;
		}
		_size++;
		return item;
	}

	DoublyLinkedListItem<T> *pop_back() {
		DoublyLinkedListItem<T> *item = 0x0;
		if (_size > 0) {
			item = _back;
			_back = _back->previous;
			_size--;
		} 
		return item;
	}

	DoublyLinkedListItem<T> *pop_front()  {
		DoublyLinkedListItem<T> *item = 0x0;
		if (_size > 0) {
			item = _front;
			_front = _front->next;
			_size--;
		} 
		return item;
	}

	void append(DoublyLinkedListItem<T> *item) {
		item->next = 0x0;
		item->previous = 0x0;
		if (_size > 0) {
			_back->next = item;
			item->previous = _back;
			_back = item;
		} else {
			_front = item;
			_back = item;
		}
		_size++;
	}

	void prepend(DoublyLinkedListItem<T> *item) {
		item->next = 0x0;
		item->previous = 0x0;
		if (_size > 0) {
			_front->previous = item;
			item->next = _front;
			_front = item;
		} else {
			_front = item;
			_back = item;
		}
		_size++;
	}

	void remove(const DoublyLinkedListItem<T> *item) {
		if (_size > 0) {
			int references = 0;
			if (_front == item) {
				_front = item->next;
				references++;
			}
			if (_back == item) {
				_back = item->previous;
				references++;
			}
			if (references == 2) {
				_front = 0x0;
				_back = 0x0;
				_size = 0;
			} else {
				if (item->previous != 0x0) {
					item->previous->next = item->next;
					references++;
				}
				if (item->next != 0x0) {
					item->next->previous = item->previous;
					references++;
				}
				if (references > 0) {
					_size--;
				}
			}
		}
	}

	DoublyLinkedListItem<T> *begin() const {
		return _front;
	}

	DoublyLinkedListItem<T> *end() const{
		return _back;
	}

	int size() const {
		return _size;
	}

private:
	DoublyLinkedListItem<T> *_front;

	DoublyLinkedListItem<T> *_back;

	int _size;

};

#endif