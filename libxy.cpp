// 生成动态库文件
// g++ -O3 -fPIC -shared libxy.cpp -o libxy.so
#include <iostream>
#include <cstring>

using namespace std;

inline long get(char l, int i, int ii)
{
    if (i == 0) return l ^ ((2001L*615*1950/195) ^ ii);
    return (l ^ (20010615*i)) ^ ii;
}

const long buffer[23] = {
    0xbbc697, 0x131560c, 0x262ac98,
    0x3940315, 0x4c559b0, 0x5f6b02c,
    0x72806ef, 0x8595d23, 0x98ab39f,
    0xabc0a52, 0xbed6081, 0xd1eb763,
    0xe500db2, 0xf81647d, 0x10b2baa5,
    0x11e41097, 0x13156751, 0x1446bd85,
    0x15781478, 0x16a96aa1, 0x17dac16e,
    0x190c17b7, 0x1a3d6e50
};

extern "C" {
    bool verify(char *flag, int len)
    {
        int i;
        for (i = 0;i < len;i++)
        {
            long f = get(flag[i], i, len);

            if (f != buffer[i]) return false;
        }
        return 0x0 == ((end(buffer) - begin(buffer)) ^ len);
    }

    void check(char *flag)
    {
        if (!verify(flag, strlen(flag)))
        {
            cout << "Verification failed, please try again." << endl;
        }
        else
        {
            cout << "Verification successful." << endl;
        }
    }
}