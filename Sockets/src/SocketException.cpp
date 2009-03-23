//
// SocketException.cpp
//
// $Id: //poco/svn/Net/src/SocketException.cpp#2 $
//
// Library: Net
// Package: NetCore
// Module:  SocketException
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


#include "Poco/Sockets/SocketException.h"
#include <typeinfo>


using Poco::IOException;


namespace Poco {
namespace Sockets {


POCO_IMPLEMENT_EXCEPTION(SocketException, IOException, "Socket Exception")
POCO_IMPLEMENT_EXCEPTION(InvalidAddressException, SocketException, "Invalid address")
POCO_IMPLEMENT_EXCEPTION(ServiceNotFoundException, SocketException, "Service not found")
POCO_IMPLEMENT_EXCEPTION(ConnectionAbortedException, SocketException, "Software caused connection abort")
POCO_IMPLEMENT_EXCEPTION(ConnectionResetException, SocketException, "Connection reset by peer")
POCO_IMPLEMENT_EXCEPTION(ConnectionRefusedException, SocketException, "Connection refused")
POCO_IMPLEMENT_EXCEPTION(DNSException, SocketException, "DNS error")
POCO_IMPLEMENT_EXCEPTION(HostNotFoundException, DNSException, "Host not found")
POCO_IMPLEMENT_EXCEPTION(NoAddressFoundException, DNSException, "No address found")
POCO_IMPLEMENT_EXCEPTION(InterfaceNotFoundException, SocketException, "Interface not found")


} } // namespace Poco::Sockets
