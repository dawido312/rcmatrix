#include <iostream>
#include "rcmatrix.h"
using namespace std;
int main()
{
rcmatrix A(5,5,5.4);
rcmatrix B(5, 3, 0.0, 6.3);
rcmatrix C(2,2);
cout <<A;
cout << endl << endl << endl;
cout <<B;
cout << endl << endl << endl;
cout <<C;
cout << endl;
rcmatrix A1(2, 3, 2, 5);
rcmatrix A2(3, 2, 5, 2);
cout << A1;
cout << endl;
cout<<A2;
cout << endl << endl << endl;
cout << A1*A2;
cout <<endl <<endl <<endl;

fstream f1;
f1.open("matrix.dat", fstream::in);
rcmatrix filematrix(f1);
cout << filematrix;
f1.close();
//cout << A[4];
cout << endl << endl;
A = B;
cout << A;
cout << endl;
cout << A[0][1];
cout << endl;
A[1][1] = 997;
cout << A;
}
