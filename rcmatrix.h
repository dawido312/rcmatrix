#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <new>
#define macierz(w,k,rozmiar) (w)*(rozmiar) + (k)
using namespace std;

class rcmatrix
{	
	struct rcdata;
	rcdata* data;
	public:
	class cref;
	class cref2;
	rcmatrix(unsigned int a, unsigned int b, double c, double d);
	rcmatrix(fstream&);
	~rcmatrix();
	friend void operator<<(ostream&, const rcmatrix&);
	friend void operator<<(ostream&, const cref2&);
	rcmatrix operator*(const rcmatrix&);
	rcmatrix::cref operator[](int i);
	rcmatrix & operator=(const rcmatrix&);
	double read(int,int);
	void write(double,int,int);
	double* getMatrixData();
	unsigned int getColumnCount();
	friend unsigned int print(const rcmatrix&);



};

struct rcmatrix::rcdata
{	
	unsigned int rows;
	unsigned int columns;
	double value;
	double value2;
	double* matrix;
	unsigned int n;
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
		n = 1;
		matrix = new double[rows*columns];
		fill(rows, columns, value, matrix, value2);
	};


	~rcdata()
	{
		delete [] matrix;
	};

	rcdata* detach()
	{
	if (n==1) return this;
	rcdata * temp = new rcdata(rows, columns);
	for (unsigned int i = 0; i < (temp->rows); i++)
		for (unsigned int j = 0; j < temp->columns; j++)
	*(temp->matrix+macierz(i,j,temp->columns)) = *(matrix+macierz(i,j,columns));
	n--;
	return temp;
	};

};

class rcmatrix::cref2
{
	friend class rcmatrix;
	friend class cref;
	public:
	rcmatrix &a;
	int i,j;
	cref2(rcmatrix& aa, int ii,int jj): a(aa), i(ii), j(jj) {};

	/*operator double() const
	{
		return a.read(i,j);	
	}*/

	void operator= (double val)
	{
		
	   a.write(val,i,j);
	}

	/*rcmatrix::cref2& operator= (const cref2& ref)
	{
	  return operator= ((double)ref);
	}*/
};

class rcmatrix::cref
{
	friend class rcmatrix;
	friend class cref2;
	public:
	rcmatrix& a;
	int i,j;
	cref(rcmatrix& aa, int ii,int jj): a(aa), i(ii), j(jj) {};
	/*operator double() const
	{
		return a.read(i,j);	
	}*/
	 rcmatrix::cref2 operator[](unsigned int j)
	{
		a.read(i,j);
		return cref2(a,i,j);
	}

	/*void operator= (double val)
	{
		
	  a.write(val,i,j);
	}*/

	//cref2(&a,i,j);
	/*rcmatrix::cref& operator= (double val)
	{
		
	  a.write(val,i,j);
	  return *this;
	}*/




};


unsigned int rcmatrix::getColumnCount() 
{
	return data->columns;
}

double* rcmatrix::getMatrixData() 
{
	return data->matrix;
}

double rcmatrix::read(int i, int j) 
{
	return *(data->matrix + macierz(i, j, data->columns));
}

void rcmatrix::write(double val,int i,int j) 
{	data = data->detach();
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
	a.data->n++;
	//rcdata* tmp = new rcdata(a.data->rows, a.data->columns,0,0);
	//data = tmp;
	//for (unsigned int i = 0; i < (a.data->rows); i++)
	//	for (unsigned int j = 0; j < a.data->columns; j++)
	//*(data->matrix+macierz(i,j,data->columns)) = *(a.data->matrix+macierz(i,j,a.data->columns));
	data = a.data;
	return *this;
}

unsigned int print(const rcmatrix& a)
{
	return (a.data->n);
}

void operator<<(ostream& c, const rcmatrix::cref2& a)
{
unsigned int b = a.i;
unsigned int d = a.j;
c <<a.a.read(b, d);
}
