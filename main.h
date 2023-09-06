#ifndef MAIN_H
typedef void (Void)(char*);
#define MAIN_H
#define LIB_NAME "libxy.so"
extern "C" {
    void __error__();
    void before();
    void after();
    void *handle;
    char *flag;
}
#endif // MAIN_H