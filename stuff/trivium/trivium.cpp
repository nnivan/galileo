#include <iostream>
#include <bitset>
#include <fstream>
using namespace std;

struct Trivium{

    string read_file(string file_name){

        ifstream in(file_name);
        string return_value;
        char c;

        if (in.is_open()){
            getline (in, return_value);
            in.close();
            return return_value;
        }
    }

    string keystr;
    string ivstr;

    Trivium(){
        keystr = read_file("trivium_key.txt");
        ivstr = read_file("trivium_iv.txt");
    }


    void setup(bitset<288> &s, bitset<3> &t, bitset<80> &KEY, bitset<80> &INITIAL_VALUE){
        for (int i = 0; i < 288; ++i){
            s[i] = 0;
        }
        t[0] = t[1] = t[2] = 0;
        for(int i=0; i<80; i++)
        {
            s[i] = KEY[i];
        }
        for(int i=0; i<80; i++)
        {
            s[i+93] = INITIAL_VALUE[i];
        }
        s[285] = 1;
        s[286] = 1;
        s[287] = 1;
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<288; i++)
            {
                t[0] = s[65]  ^ s[92];
                t[1] = s[161] ^ s[176];
                t[2] = s[242] ^ s[287];
                t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
                t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
                t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
                s <<= 1;
                s[0] = t[2];
                s[93] = t[0];
                s[177] = t[1];
            }
        }

        return;
    }

    string encrypt(string plaintext){
        string ciphertext = plaintext;

        bitset<288> s;
        bitset<3> t;
        bitset<80> key(keystr);
        bitset<80> iv(ivstr);


        setup(s, t, key, iv);

        int z = 0;
        int i,x;
        for(int x=0; x<plaintext.size(); ++x)
        {
            for(i=0; i<8; ++i)
            {
                t[0] = s[65]  ^ s[92];
                t[1] = s[161] ^ s[176];
                t[2] = s[242] ^ s[287];
                z |= (t[0] ^ t[1] ^ t[2]) << (i&0b111);

                t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
                t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
                t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
                s <<= 1;
                s[0] = t[2];
                s[93] = t[0];
                s[177] = t[1];
            }
            ciphertext[x] ^= char(z);
            z = 0;
        }

        return ciphertext;
    }
};

int main ()
{
    Trivium trivium;
    cout<<trivium.encrypt(trivium.encrypt("Hello, my friend!\n"));
    return 0;
}

