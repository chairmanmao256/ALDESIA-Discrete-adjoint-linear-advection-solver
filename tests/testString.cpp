#include <iostream>
#include <string.h>
using namespace std;

void passString(string* str_, char* c_);

int main()
{
    
    string str[2];
    str[0] = {"Hello World!\n"};
    str[1] = str[0];


    if (str[0].compare(str[1])==0){
        cout<<"Identical string!\n";
    }

    char c[20] = {"zeroGradient\n"};

    passString(str, c);
    cout<<str[0];
    cout<<str[1];


    return 0;
}

void passString(string* str_, char* c_)
{
    string* str = str_;
    char* c = c_;
    cout<<str[0];
    cout<<str[1];

    if(str[0].compare("Hello World!\n")==0){
        cout<<"Identical to a constant string!\n";
    }

    str[0] = c;
}
