#pragma once

#include <sys/types.h>
#include <string>
#define POOLSIZE 32

using namespace std;

class Person;

//Create a list node struct to store next memory location
struct FreeStore {
	FreeStore* next;
};

//Build an interface for all memory managers/
class IMemoryManager
{
public:
	virtual void* allocate(size_t) = 0;
	virtual void free(void*) = 0;
};

class MemoryManager : public IMemoryManager
{
	void expandPoolSize();
	void cleanUp();

	FreeStore* freeStoreHead;
public:
	MemoryManager(){
		freeStoreHead = nullptr;
		expandPoolSize();
	}
	virtual ~MemoryManager(){
		cleanUp();
	}
	virtual void* allocate(size_t);
	virtual void free(void*);
};


class Person {
public:
	string Name;
	int age;

	void* operator new(size_t);
	void operator delete(void*);
};

MemoryManager gMemoryManager;

void* Person::operator new(size_t size){
	return gMemoryManager.allocate(size);
}

void Person::operator delete(void* pointerToDelete){
	gMemoryManager.free(pointerToDelete);
}

void* MemoryManager::allocate(size_t size) {
	if(freeStoreHead == nullptr)
		expandPoolSize();

	FreeStore* head = freeStoreHead;
	freeStoreHead = head->next;
	return head;
}

void MemoryManager::free(void *deleted) {
	FreeStore* head = static_cast<FreeStore*> (deleted);

	head->next = freeStoreHead;
	freeStoreHead = head;
}

void MemoryManager::expandPoolSize() {
	size_t  size = (sizeof(Person) > sizeof(FreeStore*)) ? sizeof(Person) : sizeof(FreeStore*);

	FreeStore* head = reinterpret_cast<FreeStore*> (new char[size]);
	freeStoreHead = head;

	for(int i = 0; i < POOLSIZE; i++){
		head->next = reinterpret_cast<FreeStore*>(new char[size]);
		head = head->next;
	}

	head->next = nullptr;
}

void MemoryManager::cleanUp() {
	FreeStore* nextPtr = freeStoreHead;
	for(; nextPtr; nextPtr = freeStoreHead){
		freeStoreHead = freeStoreHead->next;
		delete[] nextPtr;
	}
}
