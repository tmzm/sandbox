//
// UDPEchoServer.h
//
// $Id: //poco/svn/Sockets/testsuite/src/UDPEchoServer.h#2 $
//
// Definition of the UDPEchoServer class.
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


#ifndef UDPEchoServer_INCLUDED
#define UDPEchoServer_INCLUDED


#include "Poco/Sockets/Sockets.h"
#include "Poco/Sockets/DatagramSocket.h"
#include "Poco/Sockets/SocketAddress.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"


class UDPEchoServer: public Poco::Runnable
	/// A simple sequential UDP echo server.
{
public:
	UDPEchoServer(int bufferSize = 256);
		/// Creates the UDPEchoServer.

	UDPEchoServer(const Poco::Sockets::SocketAddress& sa, int bufferSize = 256);
		/// Creates the UDPEchoServer and binds it to
		/// the given address.

	~UDPEchoServer();
		/// Destroys the UDPEchoServer.

	Poco::UInt16 port() const;
		/// Returns the port the echo server is
		/// listening on.
		
	Poco::Sockets::SocketAddress address() const;
		/// Returns the address of the server.	
		
	void run();
		/// Does the work.
		
private:
	Poco::Sockets::DatagramSocket _socket;
	Poco::Thread _thread;
	Poco::Event  _ready;
	bool         _stop;
	int          _bufferSize;
};


#endif // UDPEchoServer_INCLUDED
