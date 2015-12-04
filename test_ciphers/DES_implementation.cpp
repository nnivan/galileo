#include <iostream>
#include <time.h>
#include <cmath>
#include <random>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;

typedef unsigned long long ull;
typedef unsigned int uint;

void dec_to_binary( ull num1 )
{
   if( num1 == 0 )
   {
      return;
   }
   dec_to_binary( num1/2 );
   cout << num1 % 2;
}


class DES{

    ull key;
    ull round_key;
    uint IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                   60, 52, 44, 36, 28, 20, 12, 4,
                   62, 54, 46, 38, 30, 22, 14, 6,
                   64, 56, 48, 40, 32, 24, 16, 8,
                   57, 49, 41, 33, 25, 17, 9, 1,
                   59, 51, 43, 35, 27, 19, 11, 3,
                   61, 53, 45, 37, 29, 21, 13, 5,
                   63, 55, 47, 39, 31, 23, 15, 7};
    uint IP_R[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                     39, 7, 47, 15, 55, 23, 63, 31,
                     38, 6, 46, 14, 54, 22, 62, 30,
                     37, 5, 45, 13, 53, 21, 61, 29,
                     36, 4, 44, 12, 52, 20, 60, 28,
                     35, 3, 43, 11, 51, 19, 59, 27,
                     34, 2, 42, 10, 50, 18, 58, 26,
                     33, 1, 41, 9, 49, 17, 57, 25};
    uint E[48] = {32, 1, 2, 3, 4, 5,
                  4, 5, 6, 7, 8, 9,
                  8, 9, 10, 11, 12, 13,
                  12, 13, 14, 15, 16, 17,
                  16, 17, 18, 19, 20, 21,
                  20, 21, 22, 23, 24, 25,
                  24, 25, 26, 27, 28, 29,
                  28, 29, 30, 31, 32, 1};
    uint P[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25};

    uint S[8][64] = {
        {0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7,
         0x0,0xf,0x7,0x4,0xe,0x2,0xd,0x1,0xa,0x6,0xc,0xb,0x9,0x5,0x3,0x8,
         0x4,0x1,0xe,0x8,0xd,0x6,0x2,0xb,0xf,0xc,0x9,0x7,0x3,0xa,0x5,0x0,
         0xf,0xc,0x8,0x2,0x4,0x9,0x1,0x7,0x5,0xb,0x3,0xe,0xa,0x0,0x6,0xd},

        {0xf,0x1,0x8,0xe,0x6,0xb,0x3,0x4,0x9,0x7,0x2,0xd,0xc,0x0,0x5,0xa,
         0x3,0xd,0x4,0x7,0xf,0x2,0x8,0xe,0xc,0x0,0x1,0xa,0x6,0x9,0xb,0x5,
         0x0,0xe,0x7,0xb,0xa,0x4,0xd,0x1,0x5,0x8,0xc,0x6,0x9,0x3,0x2,0xf,
         0xd,0x8,0xa,0x1,0x3,0xf,0x4,0x2,0xb,0x6,0x7,0xc,0x0,0x5,0xe,0x9},

        {0xa,0x0,0x9,0xe,0x6,0x3,0xf,0x5,0x1,0xd,0xc,0x7,0xb,0x4,0x2,0x8,
         0xd,0x7,0x0,0x9,0x3,0x4,0x6,0xa,0x2,0x8,0x5,0xe,0xc,0xb,0xf,0x1,
         0xd,0x6,0x4,0x9,0x8,0xf,0x3,0x0,0xb,0x1,0x2,0xc,0x5,0xa,0xe,0x7,
         0x1,0xa,0xd,0x0,0x6,0x9,0x8,0x7,0x4,0xf,0xe,0x3,0xb,0x5,0x2,0xc},

        {0x7,0xd,0xe,0x3,0x0,0x6,0x9,0xa,0x1,0x2,0x8,0x5,0xb,0xc,0x4,0xf,
         0xd,0x8,0xb,0x5,0x6,0xf,0x0,0x3,0x4,0x7,0x2,0xc,0x1,0xa,0xe,0x9,
         0xa,0x6,0x9,0x0,0xc,0xb,0x7,0xd,0xf,0x1,0x3,0xe,0x5,0x2,0x8,0x4,
         0x3,0xf,0x0,0x6,0xa,0x1,0xd,0x8,0x9,0x4,0x5,0xb,0xc,0x7,0x2,0xe},

        {0x2,0xc,0x4,0x1,0x7,0xa,0xb,0x6,0x8,0x5,0x3,0xf,0xd,0x0,0xe,0x9,
         0xe,0xb,0x2,0xc,0x4,0x7,0xd,0x1,0x5,0x0,0xf,0xa,0x3,0x9,0x8,0x6,
         0x4,0x2,0x1,0xb,0xa,0xd,0x7,0x8,0xf,0x9,0xc,0x5,0x6,0x3,0x0,0xe,
         0xb,0x8,0xc,0x7,0x1,0xe,0x2,0xd,0x6,0xf,0x0,0x9,0xa,0x4,0x5,0x3},

        {0xc,0x1,0xa,0xf,0x9,0x2,0x6,0x8,0x0,0xd,0x3,0x4,0xe,0x7,0x5,0xb,
         0xa,0xf,0x4,0x2,0x7,0xc,0x9,0x5,0x6,0x1,0xd,0xe,0x0,0xb,0x3,0x8,
         0x9,0xe,0xf,0x5,0x2,0x8,0xc,0x3,0x7,0x0,0x4,0xa,0x1,0xd,0xb,0x6,
         0x4,0x3,0x2,0xc,0x9,0x5,0xf,0xa,0xb,0xe,0x1,0x7,0x6,0x0,0x8,0xd},

        {0x4,0xb,0x2,0xe,0xf,0x0,0x8,0xd,0x3,0xc,0x9,0x7,0x5,0xa,0x6,0x1,
         0xd,0x0,0xb,0x7,0x4,0x9,0x1,0xa,0xe,0x3,0x5,0xc,0x2,0xf,0x8,0x6,
         0x1,0x4,0xb,0xd,0xc,0x3,0x7,0xe,0xa,0xf,0x6,0x8,0x0,0x5,0x9,0x2,
         0x6,0xb,0xd,0x8,0x1,0x4,0xa,0x7,0x9,0x5,0x0,0xf,0xe,0x2,0x3,0xc},

        {0xd,0x2,0x8,0x4,0x6,0xf,0xb,0x1,0xa,0x9,0x3,0xe,0x5,0x0,0xc,0x7,
         0x1,0xf,0xd,0x8,0xa,0x3,0x7,0x4,0xc,0x5,0x6,0xb,0x0,0xe,0x9,0x2,
         0x7,0xb,0x4,0x1,0x9,0xc,0xe,0x2,0x0,0x6,0xa,0xd,0xf,0x3,0x5,0x8,
         0x2,0x1,0xe,0x7,0x4,0xa,0x8,0xd,0xf,0xc,0x9,0x0,0x3,0x5,0x6,0xb}};
    uint KEY_ROUND[16][48] = {
        {10,51,34,60,49,17,33,57,2,9,19,42,
         3,35,26,25,44,58,59,1,36,27,18,41,
         22,28,39,54,37,4,47,30,5,53,23,29,
         61,21,38,63,15,20,45,14,13,62,55,31},

        {2,43,26,52,41,9,25,49,59,1,11,34,
        60,27,18,17,36,50,51,58,57,19,10,33,
        14,20,31,46,29,63,39,22,28,45,15,21,
        53,13,30,55,7,12,37,6,5,54,47,23},

        {51,27,10,36,25,58,9,33,43,50,60,18,
        44,11,2,1,49,34,35,42,41,3,59,17,
        61,4,15,30,13,47,23,6,12,29,62,5,
        37,28,14,39,54,63,21,53,20,38,31,7},

        {35,11,59,49,9,42,58,17,27,34,44,2,
        57,60,51,50,33,18,19,26,25,52,43,1,
        45,55,62,14,28,31,7,53,63,13,46,20,
        21,12,61,23,38,47,5,37,4,22,15,54},

        {19,60,43,33,58,26,42,1,11,18,57,51,
        41,44,35,34,17,2,3,10,9,36,27,50,
        29,39,46,61,12,15,54,37,47,28,30,4,
        5,63,45,7,22,31,20,21,55,6,62,38},

        {3,44,27,17,42,10,26,50,60,2,41,35,
        25,57,19,18,1,51,52,59,58,49,11,34,
        13,23,30,45,63,62,38,21,31,12,14,55,
        20,47,29,54,6,15,4,5,39,53,46,22},

        {52,57,11,1,26,59,10,34,44,51,25,19,
        9,41,3,2,50,35,36,43,42,33,60,18,
        28,7,14,29,47,46,22,5,15,63,61,39,
        4,31,13,38,53,62,55,20,23,37,30,6},

        {36,41,60,50,10,43,59,18,57,35,9,3,
        58,25,52,51,34,19,49,27,26,17,44,2,
        12,54,61,13,31,30,6,20,62,47,45,23,
        55,15,28,22,37,46,39,4,7,21,14,53},

        {57,33,52,42,2,35,51,10,49,27,1,60,
        50,17,44,43,26,11,41,19,18,9,36,59,
        4,46,53,5,23,22,61,12,54,39,37,15,
        47,7,20,14,29,38,31,63,62,13,6,45},

        {41,17,36,26,51,19,35,59,33,11,50,44,
        34,1,57,27,10,60,25,3,2,58,49,43,
        55,30,37,20,7,6,45,63,38,23,21,62,
        31,54,4,61,13,22,15,47,46,28,53,29},

        {25,1,49,10,35,3,19,43,17,60,34,57,
        18,50,41,11,59,44,9,52,51,42,33,27,
        39,14,21,4,54,53,29,47,22,7,5,46,
        15,38,55,45,28,6,62,31,30,12,37,13},

        {9,50,33,59,19,52,3,27,1,44,18,41,
        2,34,25,60,43,57,58,36,35,26,17,11,
        23,61,5,55,38,37,13,31,6,54,20,30,
        62,22,39,29,12,53,46,15,14,63,21,28},

        {58,34,17,43,3,36,52,11,50,57,2,25,
        51,18,9,44,27,41,42,49,19,10,1,60,
        7,45,20,39,22,21,28,15,53,38,4,14,
        46,6,23,13,63,37,30,62,61,47,5,12},

        {42,18,1,27,52,49,36,60,34,41,51,9,
        35,2,58,57,11,25,26,33,3,59,50,44,
        54,29,4,23,6,5,12,62,37,22,55,61,
        30,53,7,28,47,21,14,46,45,31,20,63},

        {26,2,50,11,36,33,49,44,18,25,35,58,
        19,51,42,41,60,9,10,17,52,43,34,57,
        38,13,55,7,53,20,63,46,21,6,39,45,
        14,37,54,12,31,5,61,30,29,15,4,47},

        {18,59,42,3,57,25,41,36,10,17,27,50,
        11,43,34,33,52,1,2,9,44,35,26,49,
        30,5,47,62,45,12,55,38,13,61,31,37,
        6,29,46,4,23,28,53,22,21,7,63,39}};



    ull initial_permutation(ull plaitext){
        ull ciphertext = 0;
        for (uint i=0;i<64;i++) {
            ull buffer = plaitext;
            buffer = buffer >> (64 - this->IP[i]);
            //ciphertext += (buffer%2)*(1LL<<i);
            ciphertext |= (buffer & 1) << (63 - i);
        }
        return ciphertext;
    }

    ull initial_permutation_reverse(ull plaitext){
        ull ciphertext = 0;
        for(uint i=0;i<64;i++){
            ull buffer = plaitext;
            buffer = buffer >> (64 - this->IP_R[i]);
            //ciphertext += (buffer%2)*(1LL<<(63-i);
            ciphertext |= (buffer & 1) << (63 - i);
        }
        return ciphertext;
    }

    ull expansion(uint input){
        ull output = 0;
        for(uint i=0;i<48;i++){
            ull buffer = input;
            buffer = buffer >> (32 - this->E[i]);
            //output += (buffer%2)*(1LL<<(47-i));
            output |= (buffer & 1) << (47 - i);
            //dec_to_binary(output);
            //cout<<endl;
        }
        return output;
    }

    ull s_box(ull input){
        ull output = 0;
        for(int i=7;i>=0;i--){
            int r=0,c=0;
            //r = ((input>>4)&2)|(input&1);
            r |= input & 1;
            r |= ((input >> 5) & 1) << 1;
            c = (input >> 1) & 0b1111;
            //r+=(input&1)<<0; input >>= 1;

            //c+=(input&1)<<0; input >>= 1;
            //c+=(input&1)<<1; input >>= 1;
            //c+=(input&1)<<2; input >>= 1;
            //c+=(input&1)<<3; input >>= 1;
            input >>= 6;

            //r+=(input&1)<<1; input >>= 1;

            //output = output << 4*(7-i);
            output |= this->S[i][16*r + c] << (4*(7-i));

            //cout<<hex<<r<<":"<<c<<" - "<<input<<" "<<output<<endl;
        }
        return output;
    }

    ull permutation(ull input){
        ull output = 0;
        for(uint i=0;i<32;i++){
            ull buffer = input;
            buffer = buffer >> (32- this->P[i]);
            /*cout<<"-"<<P[i]<<"-: ";
            dec_to_binary(buffer);
            cout<<endl;*/
            /*dec_to_binary(buffer);
            cout<<endl;*/
            output |= (buffer & 1) << (31-i);
        }
        return output;
    }

    void rounding(uint r){
        round_key = 0;
        for(uint i=0;i<48;i++){
            ull buffer = key;
            buffer = buffer >> (64 - this->KEY_ROUND[r][i]);
            //round_key += (buffer%2)*(1LL<<i);
            round_key |= (buffer & 1) << (47-i);

        }
        //cout << hex << round_key << endl;
        return;
    }

    uint round(uint input){

        ull output = 0;

        output = expansion(input);
        output = output ^ round_key;
        output = s_box(output);
        output = permutation(output);

        return output;
        return permutation(s_box(expansion(input)^round_key));
    }

public:

    ull string_to_ull(string message){
            ull plaitext = 0;

            for(uint i=0; i < 8 and i < message.size() ; i++){
                plaitext = plaitext<<8;
                plaitext +=  reinterpret_cast<unsigned char&>(message[i]);
            }
            return plaitext;
        }

        string ull_to_string(ull message){
            string ret;

            for(uint i=0; i<8; i++){
                if((message>>((7-i)*8))&0xff) ret.push_back((message>>((7-i)*8))&0xff);
            }

            return ret;
        }

    DES(){
        set_key(0x123456789abcdef);
        //set_key(0xAABB09182736CCDD);
    }

    void set_key(ull _key){
        this->key = _key;
    }

    ull encrypt(ull plaitext){

        /*cout<<"---\n";
        dec_to_binary(expansion(0xf0aaf0aa));
        cout<<endl<<hex<<expansion(0xf0aaf0aa)<<endl;
        dec_to_binary(0x01234567);
        cout<<endl;
        for(int i = 31; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";
        cout<<"---\n";
        dec_to_binary(s_box(0x6117BA866527));
        cout<<endl<<hex<<s_box(0x6117BA866527)<<endl;
        dec_to_binary(0x6117BA866527);
        cout<<endl;
        for(int i = 47; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";
        cout<<"---\n";
        dec_to_binary(permutation(0x5C82B597));
        cout<<endl;
        dec_to_binary(0x5C82B597);
        cout<<endl;
        for(int i = 31; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";*/

        /*cout<<hex<<key<<endl;
        dec_to_binary(key);cout<<endl;

        round_key = key;
        ull buffer_key = 0;

        buffer_key = round_key&1;
        round_key >>= 1;
        for(int i=1;i<64;i++){
            if(i%8!=0){
                buffer_key *=2;
                buffer_key += round_key%2;
            }
            round_key/=2;
        }

        for(int i=0;i<56;i++){
            round_key*=2;
            round_key+=buffer_key%2;
            buffer_key/=2;
        }

        cout<<hex<<round_key<<endl;
        dec_to_binary(round_key);cout<<endl;*/

        ull ciphertext;

        ciphertext = initial_permutation(plaitext);

        ///split

        ///cout<<hex<<ciphertext<<endl;

            uint ciphertext_a, ciphertext_b;
            ciphertext_b = ciphertext;
            ciphertext = ciphertext >> 32;
            ciphertext_a = ciphertext;

        ///cout<<hex<<ciphertext_a<<ciphertext_b<<endl;

        ///split


        for(uint i=0;i<16;i++){
            rounding(i);
            ciphertext_a = ciphertext_a ^ round(ciphertext_b);
            swap(ciphertext_a,ciphertext_b);
            //cout<<hex<<ciphertext_a<<ciphertext_b<<" - "<<round_key<<endl;
            //dec_to_binary(ciphertext_b);
            //cout<<endl;
        }
        swap(ciphertext_a,ciphertext_b);

        ciphertext = 0;
        ciphertext = ciphertext_a;
        ciphertext = ciphertext << 32;
        ciphertext = ciphertext + ciphertext_b;


        ciphertext = initial_permutation_reverse(ciphertext);


        return ciphertext;
    }

    ull decrypt(ull plaitext){

        /*cout<<"---\n";
        dec_to_binary(expansion(0xf0aaf0aa));
        cout<<endl<<hex<<expansion(0xf0aaf0aa)<<endl;
        dec_to_binary(0x01234567);
        cout<<endl;
        for(int i = 31; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";
        cout<<"---\n";
        dec_to_binary(s_box(0x6117BA866527));
        cout<<endl<<hex<<s_box(0x6117BA866527)<<endl;
        dec_to_binary(0x6117BA866527);
        cout<<endl;
        for(int i = 47; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";
        cout<<"---\n";
        dec_to_binary(permutation(0x5C82B597));
        cout<<endl;
        dec_to_binary(0x5C82B597);
        cout<<endl;
        for(int i = 31; i>=0 ; i--) cout<<i%10;
        cout<<endl;
        cout<<"---\n";*/

        /*cout<<hex<<key<<endl;
        dec_to_binary(key);cout<<endl;

        round_key = key;
        ull buffer_key = 0;

        buffer_key = round_key&1;
        round_key >>= 1;
        for(int i=1;i<64;i++){
            if(i%8!=0){
                buffer_key *=2;
                buffer_key += round_key%2;
            }
            round_key/=2;
        }

        for(int i=0;i<56;i++){
            round_key*=2;
            round_key+=buffer_key%2;
            buffer_key/=2;
        }

        cout<<hex<<round_key<<endl;
        dec_to_binary(round_key);cout<<endl;*/

        ull ciphertext;

        ciphertext = initial_permutation(plaitext);

        ///split

        ///cout<<hex<<ciphertext<<endl;

            uint ciphertext_a, ciphertext_b;
            ciphertext_b = ciphertext;
            ciphertext = ciphertext >> 32;
            ciphertext_a = ciphertext;

        ///cout<<hex<<ciphertext_a<<ciphertext_b<<endl;

        ///split


        for(uint i=0;i<16;i++){
            rounding(15-i);
            ciphertext_a = ciphertext_a ^ round(ciphertext_b);
            swap(ciphertext_a,ciphertext_b);
            //cout<<hex<<ciphertext_a<<ciphertext_b<<" - "<<round_key<<endl;
            //dec_to_binary(ciphertext_b);
            //cout<<endl;
        }
        swap(ciphertext_a,ciphertext_b);

        ciphertext = 0;
        ciphertext = ciphertext_a;
        ciphertext = ciphertext << 32;
        ciphertext = ciphertext + ciphertext_b;


        ciphertext = initial_permutation_reverse(ciphertext);


        return ciphertext;
    }

    string text_encrypt_CBC(string m,ofstream &file){
        string c;

        /*std::default_random_engine generator;
        std::uniform_int_distribution<ull> distribution(0x8000000000000000,0xffffffffffffffff);
        ull dice_roll = distribution(generator);*/

        ull dice_roll = 1;


        ull l = encrypt(dice_roll);
        file<<(ull_to_string(l));

        for(int i=0;i<m.size();i+=8){

            ull e = encrypt(l ^ string_to_ull(m.substr(i,8)));

            file<<( ull_to_string(e));

            l = e;


            /*cout<<m.substr(i,8)<<endl;
            cout<<string_to_ull(m.substr(i,8))<<endl;
            cout<<encrypt(string_to_ull(m.substr(i,8)))<<endl;
            cout<<ull_to_string(encrypt(string_to_ull(m.substr(i,8))))<<endl;*/
        }

        return c;
    }
    /**string text_decrypt_CBC(string m){
        string c;

        ull l = (string_to_ull(m.substr(0,8)));

        m.erase(0,8);

        for(int i=0;i<m.size();i+=8){


            ull e = string_to_ull(m.substr(i,8));

            c.append( ull_to_string(decrypt(e) ^ l));

            l = e;
            /*cout<<m.substr(i,8)<<endl;
            cout<<string_to_ull(m.substr(i,8))<<endl;
            cout<<decrypt(string_to_ull(m.substr(i,8)))<<endl;
            cout<<ull_to_string(decrypt(string_to_ull(m.substr(i,8))))<<endl;
        }

        return c;
    }*/

};

DES test;

int main () {

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
    /*cout<< test.string_to_ull(" hello")<<endl;
    cout<< test.encrypt(test.string_to_ull(" hello"))<<endl;
    cout<< test.ull_to_string(test.encrypt(test.string_to_ull(" hello")))<<endl;
    cout<< test.decrypt(test.encrypt(test.string_to_ull(" hello")))<<endl;
    cout<< test.ull_to_string(test.decrypt(test.encrypt(test.string_to_ull(" hello"))))<<endl;
    cout<<"----------\n\n";*/


    //cout<<plaintext;

    test.text_encrypt_CBC(plaintext, myfile);
    //cout<<c<<endl;
    //cout<<"(-"<<test.text_decrypt_CBC(c)<<"-)"<<endl;


  using namespace std::chrono;

  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = chrono::duration_cast<duration<double>>(t2 - t1);

  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  using namespace std;
    is.close();
    myfile.close();
  }
  else cout << "Unable to open file";
    return 0;
}
