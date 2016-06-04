#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <tchar.h>
#include <stdio.h>

class CException
{
private:
   static const int MaxString = 1024;
public:
   hj_c8 m_pErrorStr[CException::MaxString];
   inline CException( const hj_c8* pHeader, const hj_c8* pBody, const hj_c8* pFooter )
   {
      sprintf_s( m_pErrorStr, CException::MaxString, "%s\n%s\n%s\n", pHeader, pBody, pFooter );
   }
};

#endif