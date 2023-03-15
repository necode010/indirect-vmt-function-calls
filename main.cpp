#include <iostream>
#include <windows.h>

// one flaw with this method is that every function must have at least one dummy argument,
// this could possibly be circumvented by just creating another macro

#define MAKEFN(ret_type, name, ...) virtual ret_type __stdcall name(__VA_ARGS__); typedef ret_type (__stdcall* _## name)(void* thisptr, __VA_ARGS__); uint64_t __## name;
#define VFUNC(cl, ptr, name) ((cl::_## name)((*(uintptr_t**)(ptr))[(offsetof(cl, __## name) / 8) - 1]))

class c_test {
public:
    MAKEFN(void, foo, int dummy);
    MAKEFN(int, bar, int x, int y);
    
    int _member1;
    int _member2;
};

c_test test;
c_test* g_test = &test;

void c_test::foo(int dummy) {
    _member1 = 100;
    _member2 = 200;
}

int c_test::bar(int x, int y) {
    return x + y;
}

int main() {
    g_test->_member1 = 0;
    g_test->_member2 = 0;
    
    printf("before function call foo: %i %i\n", g_test->_member1, g_test->_member2);
    VFUNC(c_test, g_test, foo)(g_test, 0);
    printf("after function call foo: %i %i\n", g_test->_member1, g_test->_member2);
    
    int result = VFUNC(c_test, g_test, bar)(g_test, 45, 30);
    printf("function call bar: %i\n", result);
    
    return 0;
}