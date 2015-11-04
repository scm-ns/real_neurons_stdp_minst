#pragma once
/*
	Martirx 
		Allocated in Heap 
		Implement matrix as a single array for preventing chache misses 
		Access to rows and col normal indexed , starts from 1 , not 0 
		Maximum Number of Elements is 10^8 ie 10,000 rows and 10,000 cols ;; If higer gives std::length_error
			//Not Implemented , change _size , _col _row to long long for larger matrices.

	Total time Taken : 12
	Started April 4th 
	End April 4th Night 

*/

//System Include 
#include <iostream> //Output
#include <ostream>
#include <string>  // Error Handling 
#include <vector>  // Internal DS 
#include <iomanip> // Output 
#include <time.h>  // randFill
#include <stdlib.h> //randFill
#include <stdio.h>  //randFill



	//------------------------------------------------------------------------------------------------

template <class T>
class matrix
{
public:
	//CONSTRUCTORS
	matrix(void) {};
	~matrix(void);
	matrix(long long rows, long long  cols):_rows(rows),_cols(cols),_matrix(_rows*_cols){ _size = _rows*_cols;} // 
	matrix(const matrix<T>& rhs);

	//GENERAL
	void print();
	long long  numRows () const {return (_rows);}; // Implicit inline 
	long long  numCols () const {return  (_cols);};
	void randFill  (T start = 0, T end = 1000); // Will only work for simple data types
	void symetricRandFill(T start = 0, T end = 1000); // Will only work for simple data types
	void setAllNum(T aNum);
	matrix<T> returnRow(int aRow);
	matrix<T> returnCol(int aCol);
	matrix<T> removeCol(int aCol);
	matrix<T> removeRow(int aRow);



	void replaceCol(matrix<T> colVec,int aCol);
	void replaceRow(matrix<T> rowVec, int aRow);
	void swap(int aRow, int aCol, int bRow, int bCol) { int temp = this->get(aRow, aCol); this->insert(aRow, aCol, this->get(bRow, bCol)); this->insert(bRow, bCol, temp); };
	// Adds a single Column to the matrix 
	matrix<T> addCol(matrix<T> col);
	matrix<T> addRow(matrix<T> row);
	bool isSquare(){ return ((_rows == _cols)? true : false );} 
	bool isRowVector() { return ((_rows == 1) ? true : false) ;}
	bool isColVector() { return ((_cols == 1) ? true : false) ;}
	bool isDiagonal();
	bool isEqual(matrix<T> rhs);
	bool isUpperTriangular();
	bool isLowerTriangular();
	bool isDiagonallyDominant();

	// Get Data from Matrix 
	
	/*
		Returns the Diagonal Elements packaged as a Column vector 
	*/
	matrix<T> getDiagonalVector();

	/*
	Returns the Diagonal Elements packaged as a Diagonal Matrix
	*/
	matrix<T> getDiagonalMatrix();


	//OPERATORS
	friend std::ostream& operator<<(std::ostream& out , matrix<T> &temp); // Not working 
	matrix<T>& operator=(const matrix<T>& rhs);
	matrix<T> operator*(const T rhs);
	matrix<T> operator/(const T rhs);
	matrix<T> operator*( matrix<T> &rhs);
	
	matrix<T> operator+( matrix<T>& rhs);
	matrix<T> operator-( matrix<T>& rhs);

	bool operator==(const matrix<T> & rhs);
	bool operator!=(const matrix<T> & rhs){ return !(*this == rhs); }

	
	T& operator()(long long  rows, long long  cols);//friend std::ostream& operator<<(std::ostream& out , matrix &temp);

	//------------------------------------------------------------------------------------------------


	// METHODS FOR SPARSE MATRIX
	bool isSparse() { return matrixSparse; }
	bool isTraDiag() { return matrixTraDiag; }
	bool isBandDiag() { return matrixBandDiag; }
	bool isTopelitz() { return matrixTopelitz; }
	
	
private:

	// CONSTRUCTORS
	//matrix(void);
	
	void init(); // sets up _matirx 

	//DATA // Make Constant in Future 
	long long  _rows; // Start Adressing from 1,1 not 0,0 ..
	long long  _cols;
	long long  _size; 
	std::vector<T> _matrix;

	// GENERAL
	T& get(long long  i ,long long  j ) { return _matrix[(i - 1)*_cols + (j -1 )]; }
	void insert(long long i, long long j, T aVaule) { _matrix[(i - 1)*_cols + (j - 1)] = aVaule; }
	static void error(const char* p ){std::string str= "matrix -> Error: "; std::cout << str << p << std::endl;}

	// SPARSE MATRIX 
	/*
		General Technique , we do not store the zero elements , rather we store only the elements that matter 
		EXAMPLE: 
			For an N x N tridiagonal matrix , we store it as a N row , 3 column matrix 
			so that each 3 parts of a tridiag is each stored as a column matrix . 

		For solving these systems we will need new techniques in la_pack
		For printing , 
			the print() command will be overloaded 
	*/
	bool matrixSparse = false; 
	bool matrixTraDiag = false;
	bool matrixBandDiag = false;
	bool matrixTopelitz = false;


	void setSparse(bool aValue){ matrixSparse = aValue; }
	void setTraDiag(bool aValue){ matrixTraDiag = aValue; }
	void setBandDiag(bool aValue){ matrixBandDiag = aValue; }
	void setTopelitz(bool aValue){ matrixTopelitz = aValue; }

	
};


// ----------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
bool matrix<T>::operator==(const matrix<T> & rhs)
{
	return this->isEqual(rhs);
}


template <class T>
void matrix<T>::symetricRandFill(T start , T end )
{
	if (_rows == _cols)
	{
		std::srand(time(0));
		for (long long i = 1; i <= _rows; i++)
		{
			for (long long j = 1; j <= _cols; j++)
			{
				if (i >= j)
				{
					_matrix[(i - 1)*_cols + (j - 1)] = static_cast<T>(std::rand() / (end - start) + 1);
				}
			}
		}

		for (long long i = 1; i <= _rows; i++)
		{
			for (long long j = 1; j <= _cols; j++)
			{
				if (j > i)
				{
					_matrix[(i - 1)*_cols + (j - 1)] = _matrix[(j - 1)*_cols + (i - 1)];
				}
			}
		}
	}
	else
	{
		error("Non Square matrix Cannot be square "); 
	}
}


template <class T>
void matrix<T>::replaceCol(matrix<T> colVec, int aCol)
/*
Replaces the specified column of matrix with a given column
Index as always starts at 1
*/
{
	if (colVec.isColVector())
	{
		for (long long i = 1; i <= _rows; i++)
		{
			for (long long j = 1; j <= _cols; j++)
			{
				if (aCol == j)
				{
					//_matrix[(i - 1)*_cols + (j - 1)] = ;
					insert(i, j, colVec(i, 1));
				}
			}
		}

	}
	else
	{
		error("replaceCol : input Vector is not Col Vec");
	}
}


template <class T>
void matrix<T>::replaceRow(matrix<T> rowVec, int aRow)
/*
Replaces the specified row of matrix with a given row
Index as always starts at 1
*/
{
	if (rowVec.isRowVector())
	{
		for (long long i = 1; i <= _rows; i++)
		{
			for (long long j = 1; j <= _cols; j++)
			{
				if (aRow == i)
				{
					//_matrix[(i - 1)*_cols + (j - 1)] = rowVec(i, 1);
					insert(i, j, rowVec(1, j));
				}
			}
		}

	}
	else
	{
		error("replaceRow : input Vector is not Row Vec");
	}
}

template <class T>
matrix<T> matrix<T>::getDiagonalVector()
{
	matrix<T> vector(_rows, 1);
		for (long long i = 1; i <= _rows; i++)
		{
			for (long long j = 1; j <= _cols; j++)
			{
				if (j == i)
				{
					//_matrix[(i - 1)*_cols + (j - 1)] = rowVec(i, 1);
					vector(i, 1) = get(i, j);
				}
			}
		}

	return vector

}

template <class T>
matrix<T> matrix<T>::getDiagonalMatrix()
{
	matrix<T> diagMatrix(_rows, _cols);
	for (long long i = 1; i <= _rows; i++)
	{
		for (long long j = 1; j <= _cols; j++)
		{
			if (j == i)
			{
				//_matrix[(i - 1)*_cols + (j - 1)] = rowVec(i, 1);
				diagMatrix(i, j) = get(i, j);
			}
		}
	}
	return diagMatrix;
}









// TYPEDEF's 

typedef matrix<double> matDouble; 
typedef matrix<int> matInt;


#include "matrix.tpp"
