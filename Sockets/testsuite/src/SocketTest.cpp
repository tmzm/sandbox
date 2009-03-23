//
// SocketTest.cpp
//
// $Id: //poco/1.3/Net/testsuite/src/SocketTest.cpp#2 $
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


#include "SocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "EchoServer.h"
#include "Poco/Sockets/StreamSocket.h"
#include "Poco/Sockets/ServerSocket.h"
#include "Poco/Sockets/SocketAddress.h"
#include "Poco/Sockets/SocketException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"
#include <iostream>


using Poco::Sockets::Socket;
using Poco::Sockets::StreamSocket;
using Poco::Sockets::ServerSocket;
using Poco::Sockets::SocketAddress;
using Poco::Sockets::ConnectionRefusedException;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;


SocketTest::SocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketTest::~SocketTest()
{
}


void SocketTest::testEcho()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));
	int n = ss.sendBytes("hello", 5);
	assert (n == 5);
	char buffer[256];
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void SocketTest::testPoll()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));
	Stopwatch sw;
	sw.start();
	Timespan timeout(1000000);
	assert (!ss.poll(timeout, Socket::SELECT_READ));
	assert (sw.elapsed() >= 900000);
	sw.restart();
	assert (ss.poll(timeout, Socket::SELECT_WRITE));
	assert (sw.elapsed() < 100000);
	ss.sendBytes("hello", 5);
	char buffer[256];
	sw.restart();
	assert (ss.poll(timeout, Socket::SELECT_READ));
	assert (sw.elapsed() < 100000);
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void SocketTest::testAvailable()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));
	Timespan timeout(1000000);
	ss.sendBytes("hello", 5);
	char buffer[256];
	assert (ss.poll(timeout, Socket::SELECT_READ));
	int av = ss.available();
	assert (av > 0 && av <= 5);
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void SocketTest::testConnect()
{
	ServerSocket serv;
	serv.bind(SocketAddress());
	serv.listen();
	StreamSocket ss;
	Timespan timeout(250000);
	ss.connect(SocketAddress("localhost", serv.address().port()), timeout);
}


void SocketTest::testConnectRefused()
{
	ServerSocket serv;
	serv.bind(SocketAddress());
	serv.listen();
	Poco::UInt16 port = serv.address().port();
	serv.close();
	StreamSocket ss;
	Timespan timeout(250000);
	try
	{
		ss.connect(SocketAddress("localhost", port));
		fail("connection refused - must throw");
	}
	catch (ConnectionRefusedException&)
	{
	}
}


void SocketTest::testConnectRefusedNB()
{
	ServerSocket serv;
	serv.bind(SocketAddress());
	serv.listen();
	Poco::UInt16 port = serv.address().port();
	serv.close();
	StreamSocket ss;
	Timespan timeout(10000);
	try
	{
		ss.connect(SocketAddress("localhost", port), timeout);
		fail("connection refused - must throw");
	}
	catch (TimeoutException&)
	{
	}
	catch (ConnectionRefusedException&)
	{
	}
}


void SocketTest::testAddress()
{
	ServerSocket serv;
	serv.bind(SocketAddress());
	serv.listen();
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", serv.address().port()));
	StreamSocket css = serv.acceptConnection();
	assert (css.peerAddress().host() == ss.address().host());
	assert (css.peerAddress().port() == ss.address().port());
}


void SocketTest::testAssign()
{
	ServerSocket serv;
	StreamSocket ss1;
	StreamSocket ss2;
	
	assert (ss1 != ss2);
	StreamSocket ss3(ss1);
	assert (ss1 == ss3);
	ss3 = ss2;
	assert (ss1 != ss3);
	assert (ss2 == ss3);
	
	try
	{
		ss1 = serv;
		fail("incompatible assignment - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}
	
	try
	{
		StreamSocket ss4(serv);
		fail("incompatible assignment - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		serv = ss1;
		fail("incompatible assignment - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}
	
	try
	{
		ServerSocket serv2(ss1);
		fail("incompatible assignment - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}
}


void SocketTest::testTimeout()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));
	
	Timespan timeout0 = ss.getReceiveTimeout();
	Timespan timeout(250000);
	ss.setReceiveTimeout(timeout);
	Timespan timeout1 = ss.getReceiveTimeout();
	std::cout << "original receive timeout:  " << timeout0.totalMicroseconds() << std::endl;
	std::cout << "requested receive timeout: " << timeout.totalMicroseconds() << std::endl;
	std::cout << "actual receive timeout:    " << timeout1.totalMicroseconds() << std::endl;
	
	// some socket implementations adjust the timeout value
	// assert (ss.getReceiveTimeout() == timeout);
	Stopwatch sw;
	try
	{
		char buffer[256];
		sw.start();
		ss.receiveBytes(buffer, sizeof(buffer));
		fail("nothing to receive - must timeout");
	}
	catch (TimeoutException&)
	{
	}
	assert (sw.elapsed() < 1000000);
	
	timeout0 = ss.getSendTimeout();
	ss.setSendTimeout(timeout);
	timeout1 = ss.getSendTimeout();
	std::cout << "original send timeout:  " << timeout0.totalMicroseconds() << std::endl;
	std::cout << "requested send timeout: " << timeout.totalMicroseconds() << std::endl;
	std::cout << "actual send timeout:    " << timeout1.totalMicroseconds() << std::endl;
	// assert (ss.getSendTimeout() == timeout);
}


void SocketTest::testBufferSize()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));
	
	int osz = ss.getSendBufferSize();
	int rsz = 32000;
	ss.setSendBufferSize(rsz);
	int asz = ss.getSendBufferSize();
	std::cout << "original send buffer size:  " << osz << std::endl;
	std::cout << "requested send buffer size: " << rsz << std::endl;
	std::cout << "actual send buffer size:    " << asz << std::endl;
	
	osz = ss.getReceiveBufferSize();
	ss.setReceiveBufferSize(rsz);
	asz = ss.getReceiveBufferSize();
	std::cout << "original recv buffer size:  " << osz << std::endl;
	std::cout << "requested recv buffer size: " << rsz << std::endl;
	std::cout << "actual recv buffer size:    " << asz << std::endl;
}


void SocketTest::testOptions()
{
	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));

	ss.setLinger(true, 20);
	bool f;
	int  t;
	ss.getLinger(f, t);
	assert (f && t == 20);
	ss.setLinger(false, 0);
	ss.getLinger(f, t);
	assert (!f);
	
	ss.setNoDelay(true);
	assert (ss.getNoDelay());
	ss.setNoDelay(false);
	assert (!ss.getNoDelay());
	
	ss.setKeepAlive(true);
	assert (ss.getKeepAlive());
	ss.setKeepAlive(false);
	assert (!ss.getKeepAlive());
	
	ss.setOOBInline(true);
	assert (ss.getOOBInline());
	ss.setOOBInline(false);
	assert (!ss.getOOBInline());
}


void SocketTest::testSelect()
{
	doSelectOrPoll1(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll1(Socket::poll);
#endif
}


void SocketTest::doSelectOrPoll1(SelectPtr pAction)
{
	Timespan timeout(250000);

	EchoServer echoServer;
	StreamSocket ss;
	ss.connect(SocketAddress("localhost", echoServer.port()));

	Socket::SocketList readList;
	Socket::SocketList writeList;
	Socket::SocketList exceptList;

	readList.push_back(ss);
	assert (pAction(readList, writeList, exceptList, timeout) == 0);
	assert (readList.empty());
	assert (writeList.empty());
	assert (exceptList.empty());
	
	ss.sendBytes("hello", 5);

	ss.poll(timeout, Socket::SELECT_READ);

	readList.push_back(ss);
	writeList.push_back(ss);
	assert (pAction(readList, writeList, exceptList, timeout) == 2);
	assert (!readList.empty());
	assert (!writeList.empty());
	assert (exceptList.empty());

	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void SocketTest::testSelect2()
{
	doSelectOrPoll2(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll2(Socket::poll);
#endif
}


void SocketTest::doSelectOrPoll2(SelectPtr pAction)
{
	Timespan timeout(100000);

	EchoServer echoServer1;
	EchoServer echoServer2;
	StreamSocket ss1(SocketAddress("localhost", echoServer1.port()));
	StreamSocket ss2(SocketAddress("localhost", echoServer2.port()));
	
	Socket::SocketList readList;
	Socket::SocketList writeList;
	Socket::SocketList exceptList;

	readList.push_back(ss1);
	readList.push_back(ss2);
	assert (pAction(readList, writeList, exceptList, timeout) == 0);
	assert (readList.empty());
	assert (writeList.empty());
	assert (exceptList.empty());
	
	ss1.sendBytes("hello", 5);

	ss1.poll(timeout, Socket::SELECT_READ);

	readList.push_back(ss1);
	readList.push_back(ss2);
	assert (pAction(readList, writeList, exceptList, timeout) == 1);

	assert (readList.size() == 1);
	assert (readList[0] == ss1);
	assert (writeList.empty());
	assert (exceptList.empty());

	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);

	readList.clear();
	writeList.clear();
	exceptList.clear();
	writeList.push_back(ss1);
	writeList.push_back(ss2);
	assert (pAction(readList, writeList, exceptList, timeout) == 2);
	assert (readList.empty());
	assert (writeList.size() == 2);
	assert (writeList[0] == ss1);
	assert (writeList[1] == ss2);
	assert (exceptList.empty());

	ss1.close();
	ss2.close();
}


void SocketTest::testSelect3()
{
	doSelectOrPoll3(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll3(Socket::poll);
#endif
}


void SocketTest::doSelectOrPoll3(SelectPtr pAction)
{
	Socket::SocketList readList;
	Socket::SocketList writeList;
	Socket::SocketList exceptList;
	Timespan timeout(1000);

	int rc = pAction(readList, writeList, exceptList, timeout);
	assert (rc == 0);
}


void SocketTest::setUp()
{
}


void SocketTest::tearDown()
{
}


CppUnit::Test* SocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketTest");

	CppUnit_addTest(pSuite, SocketTest, testEcho);
	CppUnit_addTest(pSuite, SocketTest, testPoll);
	CppUnit_addTest(pSuite, SocketTest, testAvailable);
	CppUnit_addTest(pSuite, SocketTest, testConnect);
	CppUnit_addTest(pSuite, SocketTest, testConnectRefused);
	CppUnit_addTest(pSuite, SocketTest, testConnectRefusedNB);
	CppUnit_addTest(pSuite, SocketTest, testAddress);
	CppUnit_addTest(pSuite, SocketTest, testAssign);
	CppUnit_addTest(pSuite, SocketTest, testTimeout);
	CppUnit_addTest(pSuite, SocketTest, testBufferSize);
	CppUnit_addTest(pSuite, SocketTest, testOptions);
	CppUnit_addTest(pSuite, SocketTest, testSelect);
	CppUnit_addTest(pSuite, SocketTest, testSelect2);
	CppUnit_addTest(pSuite, SocketTest, testSelect3);
	return pSuite;
}
