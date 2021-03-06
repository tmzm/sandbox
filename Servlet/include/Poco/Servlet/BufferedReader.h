//
// BufferedReader.h
//
//
// Library: Servlet
// Package: Servlet
// Module:  BufferedReader
//
// Definition of the BufferedReader class.
//
// Copyright (c) 2006, Aleksandar Fabijanic and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// The code in this file is derived from Apache Tomcat project
// Copyright (c) 2004 The Apache Software Foundation.
// http://www.apache.org/licenses/LICENSE-2.0
// 


#ifndef Servlet_BufferedReader_INCLUDED
#define Servlet_BufferedReader_INCLUDED


#include "Poco/Servlet/ServletInputStream.h"
#include "Poco/Servlet/ServletBase.h"
#include "Poco/TextEncoding.h"
#include <string>
#include <vector>
#include <map>


namespace Poco {
namespace Servlet {


class Servlet_API BufferedReader 
{
public:
	BufferedReader(ServletInputStream& istr,
		Poco::TextEncoding* pInEnc=0,
		Poco::TextEncoding* pOutEnc=0);

	~BufferedReader();

	void readLine(char b[], int off, int len);

private:
	Poco::TextEncoding* _pInEncoding;
	Poco::TextEncoding* _pOutEncoding;
	ServletInputStream& _istr;
};


} } // namespace Poco::Servlet


#endif // Servlet_BufferedReader_INCLUDED
