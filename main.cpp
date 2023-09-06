// 编译时link dl库
// g++ main.cpp -o Crackme2 -ldl -lxy -fPIC -O3

#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include "main.h"

int main(int argc, char* argv[])
{
	if (argc == 1) {
        printf("Please input flag.\n");
        return -1;
    }
    
    flag = argv[1];
}

void __error__()
{
    char *error = dlerror();
    if (error == NULL) return;
    printf("Error: %s\n", error);
	exit(-1);
}

__attribute__((destructor)) void fini()
{
    handle = dlopen(LIB_NAME, RTLD_LAZY);
	__error__();
    if (handle == NULL || flag == NULL) exit(-1);
    Void *func = (Void*) dlsym(handle, "check");
	__error__();
	func(flag);
    dlclose(handle);
}