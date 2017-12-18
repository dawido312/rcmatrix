#include <iostream>
#include <fstream>
#include <new>
#include <cstring>
#define macierz(w,k,rozmiar) (w)*(rozmiar) + (k)
using namespace std;

class rcmatrix
{
	struct rcdata;
	rcdata* data;
	public:
	class cref;
	class cref2;
	class WrongDim{};
	class IndexOutOfRange{};
	inline rcmatrix(const rcmatrix& x);
	rcmatrix(unsigned int a, unsigned int b, double c, double d);
	rcmatrix(fstream&);
	~rcmatrix();
	friend void operator<<(ostream&, const rcmatrix&);
	rcmatrix operator*(const rcmatrix&);
	cref operator[](unsigned int i);
	rcmatrix& operator=(const rcmatrix&);
	double read(unsigned int,unsigned int);
	void write(double,unsigned int,unsigned int);
	friend unsigned int print(const rcmatrix&);
/*	rcdata *temp = new rcdata(a.data->rows,a.data->columns, a.data->value,a.data->value2);
	data = temp;

	rcdata *temp = new rcdata(a.data->rows,a.data->columns,0,0);
	temp = a.data;*/


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
	public:
	rcmatrix &a;
	unsigned int i,j;
	cref2(rcmatrix& aa,unsigned int ii,unsigned int jj): a(aa), i(ii), j(jj) {};

	operator double() const
	{
		return a.read(i,j);	
	}

	void operator= (double val)
	{
		
	   a.write(val,i,j);
	}

};

class rcmatrix::cref
{
	friend class rcmatrix;
	public:
	rcmatrix& a;
	unsigned int i,j;
	cref(rcmatrix& aa,unsigned int ii,unsigned int jj): a(aa), i(ii), j(jj) {};
	 
	rcmatrix::cref2 operator[](unsigned int j)
	{
	if(j>a.data->columns) throw IndexOutOfRange();
		return cref2(a,i,j);
	}
};


double rcmatrix::read(unsigned int i,unsigned int j) 
{
	return *(data->matrix + macierz(i, j, data->columns));
}

void rcmatrix::write(double val,unsigned int i,unsigned int j) 
{	data = data->detach();
	*(data->matrix + macierz(i, j, data->columns)) = val;
}

rcmatrix::rcmatrix(unsigned int a=1, unsigned int b=1, double c=0, double d=0)
{
	data = new rcdata(a, b, c, d);
}

rcmatrix::rcmatrix(fstream& a)
{
	unsigned int z = 0;
	unsigned int x = 0;
	a >> z;
	a >> x;
	data = new rcdata(z,x);
	for (unsigned int i = 0;i<data->rows;i++)
		for(unsigned int j = 0; j<data->columns;j++)
		a >> *(data->matrix + macierz(i, j, data->columns));
}

rcmatrix::~rcmatrix()
{
  if(--data->n==0)
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
	if(data->columns!=b.data->rows) throw WrongDim();
	double s = 0;
	rcmatrix C(data->rows, b.data->columns);
	for (unsigned int i = 0;i<data->rows;i++)
		for (unsigned int j = 0; j < b.data->columns; j++)
		{
			s = 0;
			for (unsigned int k = 0; k <data->columns; k++)
			s += *(data->matrix+macierz(i,k,data->columns)) * (*(b.data->matrix+macierz(k,j,b.data->columns)));
			*(C.data->matrix+macierz(i,j,b.data->columns))= s;

		}
	return C;
}

rcmatrix::cref rcmatrix::operator[](unsigned int i)
{
	if(i>this->data->rows) throw IndexOutOfRange();
	return cref(*this,i,0);
}

rcmatrix& rcmatrix::operator=(const rcmatrix& a)
{
	a.data->n++;
 	if(--data->n == 0) delete data;
	data = a.data;
	return *this;
}

inline rcmatrix::rcmatrix(const rcmatrix& x)
  {
    x.data->n++;
    data=x.data;
  }

unsigned int print(const rcmatrix& a)
{
	return (a.data->n);
}
