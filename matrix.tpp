template <class T>
bool matrix<T>::isEqual( matrix<T> rhs)
/*
	returns True if equal , else false ; 
*/
{
	bool equal = false ; 
	if(rhs.numRows() == numRows() && rhs.numCols() == numCols())
	{
		for(long long i = 1 ; i <= _rows ;i++)
		{
			for(long long j = 1 ; j<= _cols ; j++)
			{
				if( get(i,j) == rhs(i,j)) equal = true ; 
				else return false; // is even a single element does not match false . 
			}
		}
		return equal;
	}
	else
	{
		return equal;
	}		
}




template <class T>
void matrix<T>::setAllNum(T aNum)
{
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
				get(i,j) = aNum;
		}	
	}
}

template <class T>
bool matrix<T>::isDiagonallyDominant()
{  // Diagonal element is greater than the other elements in row
	bool isDominant = false; 
	for(long long  i = 1; i <= _rows ;i++)
	{
		int diagonalElement = 0 ;  // New values for each row 
		int rowSum = 0 ; 
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			if(i == j)
				diagonalElement = get(i,j);
			else
				rowSum = rowSum + get(i,j);
		}
		if(diagonalElement > rowSum)
			isDominant = true; 
		else
			return false ;  // is even one encountered we break out . 
	}
	return isDominant;
}


template <class T>
bool matrix<T>::isUpperTriangular()
{
	if (isSquare())
	{
		bool isUpperTriangularFlag = false; 
		for(long long  i = 1; i <= _rows ;i++)
		{
			for(long long  j = 1 ; j <= _cols ; j++)
			{
				if(i > j )
				{
					if(get(i,j) == 0)
					{
						isUpperTriangularFlag = true; 
					}
					else 
					{
						isUpperTriangularFlag = false; 
						return isUpperTriangularFlag ;
					}
				} // if 	
			}// for 
		}// for
		return isUpperTriangularFlag ; // AFter going through each element	
	}else
		return false; 
}

template <class T>
bool matrix<T>::isLowerTriangular()
{
	if (isSquare()){
		bool isLowerTriangularFlag = false; 
		for(long long  i = 1; i <= _rows ;i++)
		{
			for(long long  j = 1 ; j <= _cols ; j++)
			{
				if(i < j )
				{
					if(get(i,j) == 0)
						isLowerTriangularFlag = true; 
					else 
					{
						isLowerTriangularFlag = false; 
						return isLowerTriangularFlag ;
					 }
				   }
				}
			}	
		return isLowerTriangularFlag; // AFter going through each element
	}else
		return false; 
}







template <class T>
bool matrix<T>::isDiagonal()
{
	if( isSquare())
	{
		 bool isDiagonalFlag = false;
		for(long long  i = 1; i <= _rows ;i++)
		{
			for(long long  j = 1 ; j <= _cols ; j++)
			{
				if(i == j) continue;
				if (get(i,j) == 0)
				{
					isDiagonalFlag = true; 
				}
				else
				{
					isDiagonalFlag = false;
					return isDiagonalFlag;
				}
			}	
		}
		return isDiagonalFlag;
	}else
		return false; 
}

//------------------------------------------------------------------------------------------------

template <class T>
matrix<T> matrix<T>::returnRow(int aRow)
/*
	Create a new matrix with same number of cols , but only a single row  	
*/
{
	matrix<T> result (1,_cols);
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			if (aRow == i)
			{
				result(1,j) = get(i,j); 
			}
		}	
	}
	return result ; 
}

template <class T>
matrix<T> matrix<T>::removeCol(int aCol)
/*
	No operation is done on the input matrix , the matrix returned is a new matrix 
	remove a col and returns a matrix of same size but without the specified col  
*/
{
	matrix<T> R(_rows,_cols - 1);
	for(long long  i = 1; i <= R._rows ;i++)
	{
		for(long long  j = 1 ; j <= R._cols ; j++)
		{
				if(aCol <= j)
					R(i,j) = get(i,j+1) ;
				else	
					R(i,j) = get(i,j) ;
		}	
	}
	return R ; 
}

template <class T>
matrix<T> matrix<T>::removeRow(int aRow)
/*
	No operation is done on the input matrix , the matrix returned is a new matrix 
	remove a col and returns a matrix of same size but without the specified col  
*/
{
	matrix<T> R(_rows - 1 ,_cols);
	for(long long  i = 1; i <= R._rows ;i++)
	{
		for(long long  j = 1 ; j <= R._cols ; j++)
		{
			if(aRow <= i)
				R(i,j) = get(i+1,j) ;
			else	
				R(i,j) = get(i,j) ;
		}	
	}
	return R ; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
matrix<T> matrix<T>::returnCol(int aCol)
/*
	Create a new matrix with same number of cols , but only a single row  	
*/
{
	matrix<T> result (_rows,1);
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			if (aCol == j)
			{
				result(i,1) = get(i,j); 
			}
		}	
	}
	return result ; 
}




//------------------------------------------------------------------------------------------------


template <class T>
matrix<T>::matrix(const matrix<T>& rhs)
{
	/*
		//Copy Constructor 
	*/
	_matrix = rhs._matrix;
	_rows = rhs._rows;
	_cols = rhs._cols;
}


template <class T>
matrix<T> matrix<T>::operator*( matrix<T> &rhs)
/*
	if takes the current matrix , multiplies it by the matrix on the right , and returns 
	a new matrix 
*/
{
	matrix<T> result(_rows,rhs._cols);
	 
	if(_cols == rhs._rows )
	{
				for(long long  i = 1; i <= _rows ;i++)
				{
					for(long long  j = 1 ; j <= rhs._cols ; j++)
					{
						for(long long  k = 1; k <= _cols ; k++)
						{
							result(i,j) += get(i,k) * rhs(k,j);
						}
					}		
				}
	}
	else 
	{
		error("M*M -> Rows And Col Does Not Match");
	}

	return result ; 
}



template <class T>
matrix<T> matrix<T>::operator*(const T rhs)
/*
	Multiply by scalar 
	DoesNot Modify Input Matrix
*/
{
	matrix<T> result(_rows,_cols);// rhs is a T 
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			result(i,j) = get(i,j)*rhs ;
		}	
	}
	return result;
}

template <class T>
matrix<T> matrix<T>::operator/(const T rhs)
/*
	Divide by scalar 
	DoesNot Modify Input Matrix
*/
{
	matrix<T> result(_rows,_cols);// rhs is a T 
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			result(i,j) = get(i,j) / rhs ;
		}	
	}
	return result;
}

//------------------------------------------------------------------------------------------------
// 



//------------------------------------------------------------------------------------------------

template <class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& rhs)
{
	/*
	Copy Assignment Operator 	
		Only Support Single Type Copy 
		Eg: int to int , long long  to long long  
	*/
	// Check if same 
	if(&rhs == this)
	   return *this ; 

	//Copy rows and cols
	_rows = rhs._rows;
	_cols = rhs._cols;
	_matrix = rhs._matrix;

	return *this; 
}

//------------------------------------------------------------------------------------------------

template <class T>
matrix<T> matrix<T>::operator+( matrix<T>& rhs)
{
	if(_rows == rhs._rows && _cols == rhs._cols)
	{
		matrix<T> R(_rows,_cols);
		for(long long  i = 1; i <= _rows ;i++)
		{
			for(long long  j = 1 ; j <= _cols ; j++)
			{
				R(i,j) = get(i,j) + rhs(i,j);
			}	
		}
	return R;
	}
	else
	{
		error("Not of same size ");
		return rhs;
	}
}



template <class T>
matrix<T> matrix<T>::operator-( matrix<T>& rhs)
{
	if(_rows == rhs._rows && _cols == rhs._cols)
	{
		matrix<T> R(_rows,_cols);
		for(long long  i = 1; i <= _rows ;i++)
		{
			for(long long  j = 1 ; j <= _cols ; j++)
			{
				R(i,j) = get(i,j) - rhs(i,j);
			}	
		}
	return R;
	}
	else
	{
		error("Not of same size ");
		return rhs;
	}	
}


//------------------------------------------------------------------------------------------------



template <class T>
void matrix<T>::print(){
	std::cout << std::endl << std::endl ;
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			//out << temp.get(i,j);
			std::cout << std::setw(15) << get(i,j);
			if( j == _cols ) std::cout << std::endl ; 
		}	
	}
	std::cout << std::endl ; 
}



template <class T>
std::ostream& operator<<(std::ostream& out ,matrix<T> &temp)
{
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			//out << temp.get(i,j);
			out << std::setw(7) << get(i,j);
			if( j == _cols ) out << std::endl ; 
		}	
	}
	return out ; 
}

//------------------------------------------------------------------------------------------------

template <class T>
T& matrix<T>::operator()(const long long  rows, const long long  cols)
{
	/*std::cout << "Safdsad";
	_matrix will have _rows and _cols // Do not worry about 0 index , that is handled 
	-----------------------------------*/
	if( rows > _rows || cols > _cols)
	{
		error("() -> Index out of range :: Returning "); std::cout << rows << " : " << cols << std::endl; 
		return get(1,1);
	}
	else
		return get(rows,cols);
}


//------------------------------------------------------------------------------------------------
template <class T>
void matrix<T>::randFill(T start , T end )
{
	std::srand(time(0));
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			_matrix[(i - 1)*_cols + (j -1 )] = static_cast<T>(std::rand() / (end- start) + 1)  ; 
			// Look at Stack Over Flow Question
		}	
	}	
}

//------------------------------------------------------------------------------------------------

template <class T >
matrix<T>::~matrix(void)
{
	//delete [] _matrix;
	// Memory management handled by vector class	
	_matrix.clear();
}


//------------------------------------------------------------------------------------------------

template <class T>
void matrix<T>::init(){
/*
// Initlializer called by the matrix(row,col) 
---------------------------------------------*/
	_matrix = std::vector<T>(_rows*_cols);
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			_matrix[(i - 1)*_cols + (j -1 )] = 0 ; 
		}	
	}	
}

// instead of rand use std::random . implement later 

/*
template <class T ,typename Fn ,typename ...Args>
matrix<T>::loopApply(Fn fn , Args...args )
{
	for(long long  i = 1; i <= _rows ;i++)
	{
		for(long long  j = 1 ; j <= _cols ; j++)
		{
			_matrix[(i - 1)*_cols + (j -1 )] = fn(args...) ; 
		}	
	}
}
*/