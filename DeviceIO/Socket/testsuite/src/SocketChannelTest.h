//
// SocketChannelTest.h
//
// $Id: //poco/Main/template/test.h#7 $
//
// Definition of the SocketChannelTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef SocketChannelTest_INCLUDED
#define SocketChannelTest_INCLUDED


#include "CppUnit/TestCase.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/DeviceIO/Channel.h"
#include "Poco/DeviceIO/ChannelStream.h"
#include "Poco/DeviceIO/ChannelConfig.h"
#include "Poco/DeviceIO/Socket/SocketChannel.h"


class SocketChannelTest: public CppUnit::TestCase
{
public:
	SocketChannelTest(const std::string& name);
	~SocketChannelTest();

	void testChannelStream();
	void testActiveChannelStream();
	void testStreamsStream();
	void testBinaryStream();

	void testChannelDatagram();
	void testActiveChannelDatagram();
	void testStreamsDatagram();
	void testBinaryDatagram();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void writeBinary(Poco::BinaryWriter& writer);
	void readBinary(Poco::BinaryReader& reader);
};


#endif // SocketChannelTest_INCLUDED