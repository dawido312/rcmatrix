#include <iostream>
#include <fstream>
#include <stdio.h>
#define macierz(w,k,rozmiar) (w)*(rozmiar) + (k)
using namespace std;

class rcmatrix
{	
	struct rcdata;
	rcdata* data;
	public:
	class cref;
	rcmatrix(unsigned int a, unsigned int b, double c, double d);
	rcmatrix(fstream&);
	~rcmatrix();
	friend void operator<<(ostream&, const rcmatrix&);
	rcmatrix operator*(const rcmatrix&);
	rcmatrix::cref operator[](int i);
	rcmatrix & operator=(const rcmatrix&);
	double read(int,int);
	void write(double,int,int);
	double* getMatrixData();
	unsigned int getColumnCount();
	class cref;



};

struct rcmatrix::rcdata
{	//TO JEST WERSJA BEZ ZLICZANIA
	unsigned int rows;
	unsigned int columns;
	double value;
	double value2 = 0;
	double* matrix;
	void fill(unsigned int rows, unsigned int columns, double value, double*matrix, double value2)
	{
		for (unsigned int i = 0; i < rows; i++)
			for (unsigned int j = 0; j < columns; j++)
			{
				if (i == j) *(matrix + macierz(i, j, columns)) = value;
				else *(matrix + macierz(i, j, columns)) = value2;
			}
	}

	rcdata(unsigned int a = 1, unsigned int b = 1, double c = 0, double d = 0)
	{
		rows = a;
		columns = b;
		value = c;
		value2 = d;
		matrix = new double[rows*columns];
		fill(rows, columns, value, matrix, value2);
	};

	~rcdata()
	{
		delete [] matrix;
	};
};

class rcmatrix::cref
{
	friend class rcmatrix;
	public:
	rcmatrix& a;
	int i,j;
	cref(rcmatrix& aa, int ii,int jj): a(aa), i(ii), j(jj) {};
	operator double() const {
		return a.read(i,j);	
	}
	double& operator[](unsigned int j)
	{
		return *(a.getMatrixData() + macierz(i,j,a.getColumnCount()));
	}
	rcmatrix::cref& operator= (double val)
	{
	  cout << "void operator= "<<endl;
	  a.write(val,i,j);
	  return *this;
	}
	rcmatrix::cref& operator= (const cref& ref)
	{
	  return operator= ((double)ref);
	}



};

unsigned int rcmatrix::getColumnCount() {
	return data->columns;
}

double* rcmatrix::getMatrixData() {
	return data->matrix;
}

double rcmatrix::read(int i, int j) {
	return *(data->matrix + macierz(i, j, data->columns));
}

void rcmatrix::write(double val,int i,int j) {
	*(data->matrix + macierz(i, j, data->columns)) = val;
}

rcmatrix::rcmatrix(unsigned int a =1, unsigned int b = 1, double c = 0, double d=0)
{
	data = new rcdata(a, b, c, d);
}

rcmatrix::rcmatrix(fstream& a)
{
	int z = 0;
	int x = 0;
	a >> z;
	a >> x;
	data = new rcdata(z,x);
	for (unsigned int i = 0;i<data->rows;i++)
		for(unsigned int j = 0; j<data->columns;j++)
		a >> *(data->matrix + macierz(i, j, data->columns));
}

rcmatrix::~rcmatrix()
{
	delete data;
}

void operator << (ostream& c, const rcmatrix& p)
{	

	for (unsigned int i = 0; i < (p.data->rows); i++)
	{
		for (unsigned int j = 0; j < p.data->columns; j++)
		{
			c << *(p.data->matrix + macierz(i, j, p.data->columns)) << "\t";
		}
		c << endl;
	}
}

rcmatrix rcmatrix::operator*(const rcmatrix& b)
{
	//if(this->data->columns!=a.data->rows) throw (WrongDim&)
	double s = 0;
	unsigned int c = this->data->rows;
	unsigned int d = b.data->columns;
	rcmatrix C(c, d);
	for (unsigned int i = 0;i<this->data->rows;i++)
		for (unsigned int j = 0; j < b.data->columns; j++)
		{
			s = 0;
			for (unsigned int k = 0; k < this->data->columns; k++)
			s += *(this->data->matrix+macierz(i,k,this->data->columns)) * (*(b.data->matrix+macierz(k,j,b.data->columns)));
			*(C.data->matrix+macierz(i,j,b.data->columns))= s;

		}
	return C;
}

rcmatrix::cref rcmatrix::operator[](int i)
{
	//double a = *(data->matrix+macierz(i,0,data->columns));
	return cref(*this,i,0);
}

rcmatrix& rcmatrix::operator=(const rcmatrix& a)
{
	delete this->data;
	rcdata* tmp = new rcdata(a.data->rows, a.data->columns, 0, 0);
	data = tmp;
	for (unsigned int i = 0; i < (a.data->rows); i++)
		for (unsigned int j = 0; j < a.data->columns; j++)
	*(data->matrix+macierz(i,j,data->columns)) = *(a.data->matrix+macierz(i,j,a.data->columns));
	return *this;
}
