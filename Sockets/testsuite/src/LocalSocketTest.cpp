//
// LocalSocketTest.cpp
//
// $Id: //poco/1.3/Net/testsuite/src/LocalSocketTest.cpp#2 $
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


#include "LocalSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "EchoServer.h"
#include "Poco/Sockets/StreamSocket.h"
#include "Poco/Sockets/ServerSocket.h"
#include "Poco/Sockets/SocketAddress.h"
#include "Poco/Sockets/SocketException.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/Stopwatch.h"
#include "Poco/Environment.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include <iostream>
#include <sstream>


using Poco::Sockets::Socket;
using Poco::Sockets::StreamSocket;
using Poco::Sockets::ServerSocket;
using Poco::Sockets::SocketAddress;
using Poco::Sockets::ConnectionRefusedException;
using Poco::Sockets::SocketException;
using Poco::Timespan;
using Poco::Timestamp;
using Poco::Stopwatch;
using Poco::Environment;
using Poco::File;
using Poco::FileOutputStream;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;


LocalSocketTest::LocalSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


LocalSocketTest::~LocalSocketTest()
{
}


void LocalSocketTest::testSocketsPerformance()
{
	Timestamp::TimeDiff local = 0, net = 0;
	std::size_t initBufSize = 1;
	std::size_t initReps = 1;
	bool noDelay[2] = { true, false };

	std::cout << std::endl << "OS Name:         " << Environment::osName() << std::endl;
	std::cout << "OS Version:      " << Environment::osVersion() << std::endl;
	std::cout << "OS Architecture: " << Environment::osArchitecture() << std::endl;

	for (int d = 0; d < 2; ++d)
	{
		double locData = 0.0, locTime = 0.0, netData = 0.0, netTime = 0.0;
		std::ostringstream os;
		os << Environment::osName() << '-' 
			<< Environment::osVersion() << '-' 
			<< Environment::osArchitecture() << "-TCP";
		if (noDelay[d]) os << "-nodelay";
		os << ".csv";
		File f(os.str());
		if (f.exists()) f.remove();
		FileOutputStream fos(os.str());

		for (std::size_t repetitions = initReps;
				repetitions <= 100000;
				repetitions *= 10)
		{
			for (std::size_t bufSize = initBufSize; bufSize < 20000; bufSize *= 2)
			{
				char* pBuf = new char[bufSize];
				{
					SocketAddress sas("/tmp/poco.server.tcp.sock");
					EchoServer echoServer(sas, bufSize);
					SocketAddress sac("/tmp/poco.client.tcp.sock");
					StreamSocket ss(sas, &sac);
					int recv = 0, sent = 0;
					Stopwatch sw; int i = 0;
					for (; i < repetitions; ++i)
					{
						sent = 0; recv = 0; local = 0;

						do
						{
							int s;
							sw.restart();
							s = ss.sendBytes(pBuf + sent, bufSize - sent);
							sw.stop();
							local += sw.elapsed();
							sent += s;
						} while (sent < bufSize);

						do
						{
							int r;
							sw.restart();
							r = ss.receiveBytes(pBuf + recv, bufSize - recv);
							sw.stop();
							local += sw.elapsed();
							recv += r;
						} while (recv < bufSize);

						locData += sent;
						locData += recv;
						locTime += local;

						poco_assert (sent == bufSize && recv == bufSize);
					}
					
					std::cout << "Local TCP socket, " << i << " repetitions, " << bufSize << " bytes, " 
						<< local << " [us]." << std::endl;
					ss.close();
				}

				{
					SocketAddress sa("localhost", 12345);
					EchoServer echoServer(sa, bufSize);
					StreamSocket ss;
					ss.connect(SocketAddress(sa.host(), echoServer.port()));
					if (noDelay[d]) ss.setNoDelay(true);
					int recv = 0, sent = 0;
					Stopwatch sw; int i = 0; 
					for (; i < repetitions; ++i)
					{
						sent = 0; recv = 0; net = 0;
						do
						{
							int s;
							sw.restart();
							s = ss.sendBytes(pBuf + sent, bufSize - sent);
							sw.stop();
							net += sw.elapsed();
							sent += s;
						} while (sent < bufSize);

						do
						{
							int r;
							sw.restart();
							r = ss.receiveBytes(pBuf + recv, bufSize - recv);
							sw.stop();
							net += sw.elapsed();
							recv += r;
						} while (recv < bufSize);

						netData += sent;
						netData += recv;
						netTime += net;

						poco_assert (sent == bufSize && recv == bufSize);
					}
					
					std::cout << "Network TCP socket, " << i << " repetitions, " << bufSize << " bytes, " 
						<< net << " [us]." << std::endl;
					fos << i << ',' << bufSize << ',';
					ss.close();
				}
				delete pBuf;

				double ratio = ((double) net) / ((double) local);
				double diff = ((double) net) - ((double) local);
				std::cout << "Ratio: " << ratio << "(" << diff / 1000.0 << "[us/msg]" << std::endl;

				fos << ratio << std::endl;
			}
		}
		poco_assert (locData == netData);

		double locDTR = ((locData / (locTime / Timestamp::resolution())) * 8) / 1000000;
		double netDTR = ((netData / (netTime / Timestamp::resolution())) * 8) / 1000000;

		std::cout << (d ? "NO DELAY" : "DELAY") << std::endl
			<< "=================================" << std::endl
			<< "Local DTR: " << ((locData / (locTime / Timestamp::resolution())) * 8) / 1000000 << " [Mbit/s]" << std::endl
			<< "Network DTR: " << ((netData / (netTime / Timestamp::resolution())) * 8) / 1000000 << " [Mbit/s]" << std::endl
			<< "Local sockets speedup: " << ((locDTR / netDTR) * 100) - 100 << '%' << std::endl
			<< "=================================" << std::endl;
		
		fos << "=================================" << std::endl
			<< "Local DTR: " << ((locData / (locTime / Timestamp::resolution())) * 8) / 1000000 << " [Mbit/s]" << std::endl
			<< "Network DTR: " << ((netData / (netTime / Timestamp::resolution())) * 8) / 1000000 << " [Mbit/s]" << std::endl
			<< "Local sockets speedup: " << ((locDTR / netDTR) * 100) - 100 << '%' << std::endl
			<< "=================================" << std::endl;

		fos.close();
	}
}


void LocalSocketTest::testEcho()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);
	
	int n = ss.sendBytes("hello", 5);
	assert (n == 5);
	char buffer[256];
	n = ss.receiveBytes(buffer, sizeof(buffer));
		
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void LocalSocketTest::testPoll()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);
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


void LocalSocketTest::testAvailable()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);
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


void LocalSocketTest::testConnect()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	ServerSocket serv(sas);
	StreamSocket ss;
	Timespan timeout(250000);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	ss.connect(sas, timeout, &sac);
}


void LocalSocketTest::testConnectRefused()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	ServerSocket serv(sas);
	serv.close();
	StreamSocket ss;
	Timespan timeout(250000);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	try
	{
		ss.connect(sas, timeout, &sac);
		fail("connection refused - must throw");
	}
	catch (ConnectionRefusedException&)
	{
	}
}


void LocalSocketTest::testConnectRefusedNB()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	ServerSocket serv;
	serv.bind(sas);
	serv.listen();
	serv.close();
	StreamSocket ss;
	Timespan timeout(10000);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	try
	{
		ss.connect(sas, timeout, &sac);
		fail("connection refused - must throw");
	}
	catch (TimeoutException&)
	{
	}
	catch (ConnectionRefusedException&)
	{
	}
}


void LocalSocketTest::testAddress()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	ServerSocket serv;
	serv.bind(sas);
	serv.listen();
	StreamSocket ss;
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	ss.connect(sas, &sac);
	StreamSocket css = serv.acceptConnection();
	assert (css.peerAddress().host() == ss.address().host());
	assert (css.peerAddress().port() == ss.address().port());
}


void LocalSocketTest::testTimeout()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);
	
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


void LocalSocketTest::testBufferSize()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);
	
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


void LocalSocketTest::testOptions()
{
	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);

	ss.setLinger(true, 20);
	bool f;
	int  t;
	ss.getLinger(f, t);
	assert (f && t == 20);
	ss.setLinger(false, 0);
	ss.getLinger(f, t);
	assert (!f);
	
	//NB: these probably should have no effect for local sockets
	ss.setKeepAlive(true);
	assert (ss.getKeepAlive());
	ss.setKeepAlive(false);
	assert (!ss.getKeepAlive());
	
	ss.setOOBInline(true);
	assert (ss.getOOBInline());
	ss.setOOBInline(false);
	assert (!ss.getOOBInline());
}


void LocalSocketTest::testSelect()
{
	doSelectOrPoll1(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll1(Socket::poll);
#endif
}


void LocalSocketTest::doSelectOrPoll1(SelectPtr pAction)
{
	Timespan timeout(250000);

	SocketAddress sas("/tmp/poco.server.tcp.sock");
	EchoServer echoServer(sas);
	SocketAddress sac("/tmp/poco.client.tcp.sock");
	StreamSocket ss(sas, &sac);

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


void LocalSocketTest::testSelect2()
{
	doSelectOrPoll2(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll2(Socket::poll);
#endif
}


void LocalSocketTest::doSelectOrPoll2(SelectPtr pAction)
{
	Timespan timeout(100000);

	SocketAddress sas1("/tmp/poco.server1.tcp.sock");
	EchoServer echoServer1(sas1);
	SocketAddress sac1("/tmp/poco.client1.tcp.sock");
	StreamSocket ss1(sas1, &sac1);

	SocketAddress sas2("/tmp/poco.server2.tcp.sock");
	EchoServer echoServer2(sas2);
	SocketAddress sac2("/tmp/poco.client2.tcp.sock");
	StreamSocket ss2(sas2, &sac2);
	
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


void LocalSocketTest::testSelect3()
{
	doSelectOrPoll3(Socket::select);
#if defined(POCO_HAVE_FD_POLL)
	doSelectOrPoll3(Socket::poll);
#endif
}


void LocalSocketTest::doSelectOrPoll3(SelectPtr pAction)
{
	Socket::SocketList readList;
	Socket::SocketList writeList;
	Socket::SocketList exceptList;
	Timespan timeout(1000);

	int rc = pAction(readList, writeList, exceptList, timeout);
	assert (rc == 0);
}


void LocalSocketTest::setUp()
{
}


void LocalSocketTest::tearDown()
{
}


CppUnit::Test* LocalSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LocalSocketTest");

	CppUnit_addTest(pSuite, LocalSocketTest, testSocketsPerformance);
	CppUnit_addTest(pSuite, LocalSocketTest, testEcho);
	CppUnit_addTest(pSuite, LocalSocketTest, testPoll);
	CppUnit_addTest(pSuite, LocalSocketTest, testAvailable);
	CppUnit_addTest(pSuite, LocalSocketTest, testConnect);
	CppUnit_addTest(pSuite, LocalSocketTest, testConnectRefused);
	CppUnit_addTest(pSuite, LocalSocketTest, testConnectRefusedNB);
	CppUnit_addTest(pSuite, LocalSocketTest, testAddress);
	CppUnit_addTest(pSuite, LocalSocketTest, testTimeout);
	CppUnit_addTest(pSuite, LocalSocketTest, testBufferSize);
	CppUnit_addTest(pSuite, LocalSocketTest, testOptions);
	CppUnit_addTest(pSuite, LocalSocketTest, testSelect);
	CppUnit_addTest(pSuite, LocalSocketTest, testSelect2);
	CppUnit_addTest(pSuite, LocalSocketTest, testSelect3);
	return pSuite;
}
