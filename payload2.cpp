#include <dlfcn.h>
#include <iostream>

using namespace std;

int get(long l, int i, int ii)
{
    if (i == 0) return l ^ ((2001L*615*1950/195) ^ ii);
    return (l ^ (20010615*i)) ^ ii;
}

int main()
{
    void *handle = dlopen("libxy.so", RTLD_LAZY);
    if (handle == NULL) exit(-1);
    long *buffer = (long*) dlsym(handle, "buffer");
    char flag[23] = {};
	for (int i = 0;buffer[i] > 0;i++)
    {
        flag[i] = get(buffer[i], i, 23);
    }
	cout << flag << endl;;
    dlclose(handle);
}