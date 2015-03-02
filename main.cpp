#include <iostream>
#include "MemoryManager.h"

using namespace std;

int main() {

	Person* p = new Person();

	delete p;

	return 0;
}