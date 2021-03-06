//
// SSHOperation.h
//
// $Id: //poco/Main/SSH/include/Poco/SSH/SSHOperation.h#1 $
//
// Library: SSH
// Package: SSHCore
// Module:  SSHOperation
//
// Definition of the SSHOperation class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#ifndef SSH_SSHOperation_INCLUDED
#define SSH_SSHOperation_INCLUDED


#include "Poco/SSH/SSH.h"


namespace Poco {
namespace SSH {


	class SSHSession;


class SSH_API SSHOperation
	/// Abstract super class for all SSHOperations
{
public:
	SSHOperation();
		/// Creates the SSHOperation.

	virtual ~SSHOperation();
		/// Destroys the SSHOperation.

	void init(SSHSession& session);
		/// Initializes the operation, throws an exception if 
		/// initialization failed

	bool isInitialized() const;
		/// returns true if the operation was successfully initialized

	void close();
		/// Shutsdown the operation. After close isInitialized must return false

protected:
	virtual void initImpl() = 0;
		/// Initializes the operation, throws an exception if 
		/// initialization failed

	virtual void closeImpl() = 0;
		/// Shutsdown the operation. After close isInitialized must return false

	SSHSession& session() const;
		/// Will fail if the operation is not initialized

private:
	SSHSession* _pSession;
};


inline SSHSession& SSHOperation::session() const
{
	poco_check_ptr (_pSession);
	return *_pSession;
}


inline bool SSHOperation::isInitialized() const
{
	return (_pSession != 0);
}


} } // namespace Poco::SSH


#endif // SSH_SSHOperation_INCLUDED
