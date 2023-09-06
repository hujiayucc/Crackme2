#include <iostream>

using namespace std;

const long buffer[23] = {
    0xbbc697, 0x131560c, 0x262ac98,
    0x3940315, 0x4c559b0, 0x5f6b02c,
    0x72806ef, 0x8595d23, 0x98ab39f,
    0xabc0a52, 0xbed6081, 0xd1eb763,
    0xe500db2, 0xf81647d, 0x10b2baa5,
    0x11e41097, 0x13156751, 0x1446bd85,
    0x15781478, 0x16a96aa1, 0x17dac16e,
    0x190c17b7, 0x1a3d6e50,
};

int get(long l, int i)
{
    if (i == 0) return (23 ^ (0xc6e6 | 0xbb0000) ^ l);
    else return (i * (0x5677 | 0x1310000) ^ l ^ 23);
}

int main()
{
	for (int i = 0;i < 23;i++)
    {
        cout << ((char) get(buffer[i], i));
    }
    
    cout << endl;
}