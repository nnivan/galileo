#include <iostream>
#include <bitset>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;

void setup(bitset<288> &s, bitset<3> &t, bitset<80> &KEY, bitset<80> &INITIAL_VALUE)
{
    for (int i = 0; i < 288; ++i) s[i] = 0;
    t[0] = t[1] = t[2] = 0;
    for(int i=0; i<80; i++)
    {
        s[i] = KEY[i];
    }
    for(int i=0; i<80; i++)
    {
        s[i] = INITIAL_VALUE[i];
    }
    s[285] = 1;
    s[286] = 1;
    s[287] = 1;
    int output = 0;
    for(int j=0; j<4; j++)
    {
        for(int i=0; i<288; i++)
        {
            t[0] = s[65]  ^ s[92];
            t[1] = s[161] ^ s[176];
            t[2] = s[242] ^ s[287];
            output |= (t[0] ^ t[1] ^ t[2]) << (i%8);
            if(i%8==7)
            {
                output = 0;
            }

            t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
            t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
            t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
            s <<=1;
            s[0] = t[2];
            s[93] = t[0];
            s[177] = t[1];
        }
    }

    return;
}

int main ()
{

    string inputFile;
    inputFile = "input.txt";//cin>>inputFile;
    std::ifstream is(inputFile);
    char cc;
    //cin>>inputFile;
    inputFile = "output.txt";//cin>>inputFile;

  ofstream myfile (inputFile);
  if (myfile.is_open() and is.is_open())
  {

    string plaintext;
    while(is.get(cc))
    {
        plaintext.push_back(cc);
    }
  using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();


  using namespace std;



    bitset<288> s;
    bitset<3> t;
    bitset<80> key("00000001000000000000000000000000000000000000000000000000000000000000000000000000");
    bitset<80> iv ("00000000000000000000000000000000000000000000000000000000000000000000000000000000");

    setup(s, t, key, iv);

    int output = 0;
    int i,x;
    for(int x=0;x<plaintext.size();++x){
        for(i=0; i<8; ++i)
        {
            t[0] = s[65]  ^ s[92];
            t[1] = s[161] ^ s[176];
            t[2] = s[242] ^ s[287];
            output |= (t[0] ^ t[1] ^ t[2]) << (i&0b111);

            t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
            t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
            t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
            s <<=1;
            s[0] = t[2];
            s[93] = t[0];
            s[177] = t[1];
        }
        myfile<<char(char(output) ^ plaintext[x]);
        output = 0;
    }
  using namespace std::chrono;

  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = chrono::duration_cast<duration<double>>(t2 - t1);

  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  using namespace std;
    myfile.close();
    is.close();
  }
  else cout << "Unable to open file";
    return 0;
}
