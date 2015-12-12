#ifndef DOUBLY_LINKED_LIST_ITEM_HPP
#define DOUBLY_LINKED_LIST_ITEM_HPP

template<class T> class DoublyLinkedListItem {

public:
	DoublyLinkedListItem<T> *previous;
	DoublyLinkedListItem<T> *next;

	T value;

	DoublyLinkedListItem(DoublyLinkedListItem<T> *previous, DoublyLinkedListItem<T> *next, const T &value) :
	previous(previous),
	next(next), 
	value(value) {

	}

};

#endif