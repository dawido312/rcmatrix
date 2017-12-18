#include <iostream>
#include "rcmatrix.h"
using namespace std;

int main()
{

	try	
	{
		rcmatrix A(2,5,1.0);
		rcmatrix B(5, 3, 0.0, 6.3);
		rcmatrix C(3, 3, 1.0, 1.3);
		cout <<"Macierz A: " <<endl;
		cout <<A;
		cout << endl << endl << endl;
		cout <<"Macierz B: " <<endl;
		cout <<B;
		cout << endl <<endl <<endl;
		rcmatrix A1(2, 3, 2, 5);
		rcmatrix A2(3, 2, 5, 2);
		cout <<"Macierz A1: " <<endl;
		cout << A1;
		cout << endl;
		cout <<"Macierz A2: "<<endl;
		cout<<A2;
		cout << endl << endl << endl;
		cout <<"Po przemnozeniu: " <<endl;
		rcmatrix S = A1*A2;
		cout << S;
		cout <<endl <<endl <<endl;
		fstream f1;
		f1.open("matrix.dat", fstream::in);
		rcmatrix filematrix(f1);
		cout <<"Macierz wczytana z pliku: "<<endl;
		cout << filematrix;
		f1.close();
		cout << endl << endl;
		C = A;
		cout << C;
		A = B;
		C=B;
		cout << A;
		cout << C;

		cout << endl << endl;
		cout <<print(A);
		cout << endl;
		A[1][1] = 1.4;
		cout <<"A[1][1] = "<< A[1][1] <<endl;
		cout <<"B[1][1] = "<<B[1][1] <<endl;
		cout <<endl <<print(A);
		cout <<endl;
		}
		catch(rcmatrix::IndexOutOfRange&)
		{
			cout << "Index Out of Range" << endl;
		}
		catch(rcmatrix::WrongDim&)
		{
			cout << "Wrong Matrix Dimensions" << endl;
		}
		catch(bad_alloc)
		{
			cout << "Out of Memory" << endl;
		}


}
