//
// SocketsTestSuite.cpp
//
// $Id: //poco/svn/Net/testsuite/src/SocketsTestSuite.cpp#2 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
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


#include "SocketTestSuite.h"
#include "SocketTest.h"
#include "SocketStreamTest.h"
#include "DatagramSocketTest.h"
#ifdef POCO_OS_FAMILY_UNIX
#	include "LocalSocketTest.h"
#	include "DatagramLocalSocketTest.h"
#endif
#include "NetworkInterfaceTest.h"
#include "MulticastSocketTest.h"
#include "DialogSocketTest.h"
#include "RawSocketTest.h"


CppUnit::Test* SocketTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketTestSuite");

	pSuite->addTest(SocketTest::suite());
	pSuite->addTest(SocketStreamTest::suite());
	pSuite->addTest(DatagramSocketTest::suite());
#ifdef POCO_OS_FAMILY_UNIX
	pSuite->addTest(DatagramLocalSocketTest::suite());
	pSuite->addTest(LocalSocketTest::suite());
#endif
	pSuite->addTest(NetworkInterfaceTest::suite());
	pSuite->addTest(MulticastSocketTest::suite());
	pSuite->addTest(DialogSocketTest::suite());
	pSuite->addTest(RawSocketTest::suite());

	return pSuite;
}
