
#include <stdint.h>
#include <iostream>
#include <string.h>
#include "sha512.h"
using namespace std;


int main()
{
	SHA512Context sha;
	int i, j, err;
	char a[100000];
	cin>>a;
	std::string h=getHash(a);
	cout<<h<<endl;

	return 0;
}