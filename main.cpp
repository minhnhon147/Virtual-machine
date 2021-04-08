
#include "VM.h"
//#include "VM.cpp"
using namespace std;

void test(string filename) {
    VM* vm = new VM();
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main()
{
    test("Text.txt");
    return 0;
}