//--------------------------------------------------------------
/*!	@brief	SharedPtr

	@author 
	
	Copyright (C) 2015 HonjoTadashi. All Rights Reserved. 
*///------------------------------------------------------------
#ifndef __HJ_SHARED_PTR_H_
#define __HJ_SHARED_PTR_H_

// HjLib namespace
namespace hj{
	
template<class T>
class clsHJSharedPtr
{
public:

    clsHJSharedPtr() 
		: m_ptr(NULL), m_pCount(NULL)
	{
	}
    clsHJSharedPtr(const clsHJSharedPtr<T>& src)
		: m_ptr(NULL), m_pCount(NULL)
	{
		CopyImpl(src);
	}

    explicit clsHJSharedPtr(T* ptr)
		: m_ptr(NULL), m_pCount(NULL)
	{
		Reset( ptr );
	}
    ~clsHJSharedPtr()
	{
		DeleteCheck();
	}

	void Reset( T* ptr=NULL )
	{
		DeleteCheck();

		if( ptr ){
			m_ptr = ptr;
			m_pCount = NEW hj_int(1);
		}
	}

    // === Operator
    T* operator ->() const
	{
		HJ_ASSERT( m_ptr!=NULL, HJ_T("invalid pointer!") );
		return m_ptr; 
	}
    operator T*() const{
		return m_ptr; 
	}
    T& operator *() const{
		HJ_ASSERT( m_ptr!=NULL, HJ_T("invalid pointer!") );
		return *m_ptr; 
	}
	clsHJSharedPtr<T>& operator =(const clsHJSharedPtr<T>& src)
	{
		// コピー先がNULLなら参照無しということにしておく
		if( src.m_ptr ){
			CopyImpl(src);
		}else{
			DeleteCheck();
		}
	    return *this;
	}

	// === Member Function
    T* Get() const { return m_ptr; }
    hj_int GetUseCount() const { return (m_ptr == NULL) ? 0 : (*m_pCount); }

private:
    void CopyImpl(const clsHJSharedPtr<T>& src);
    void DeleteCheck();
	
    T*		m_ptr;
    hj_int*	m_pCount;
};

template<class T>
void clsHJSharedPtr<T>::CopyImpl(const clsHJSharedPtr<T>& src)
{
    if (this != &src)
    {
        if (m_ptr != src.m_ptr)
        {
            DeleteCheck();

            if (src.m_ptr != NULL)
            {
                m_ptr = src.m_ptr;
                m_pCount = src.m_pCount;
                (*m_pCount)++;
            }
        }
    }
}

template<class T>
void clsHJSharedPtr<T>::DeleteCheck()
{
    if( m_ptr != NULL )
    {
        (*m_pCount)--;
        if( (*m_pCount) == 0 )
        {
			HJ_SAFE_DELETE( m_ptr );
			HJ_SAFE_DELETE( m_pCount );
		}
		else
		{
			m_ptr = NULL;
			m_pCount = NULL;
		}
    }
}
	
} //namespace hj

#endif // __HJ_SHARED_PTR_H_