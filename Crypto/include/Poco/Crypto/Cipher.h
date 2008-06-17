//
// Cipher.h
//
// $Id$
//
// Library: Crypto
// Package: CryptoCore
// Module:	Cipher
//
// Definition of the Cipher class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#ifndef Crypto_Cipher_INCLUDED
#define Crypto_Cipher_INCLUDED


#include "Poco/Crypto/Crypto.h"

#include <vector>


namespace Poco {
namespace Crypto {


class CryptoTransform;


class Crypto_API Cipher
	/// Represents the abstract base class from which all implementations of
	/// symmetric encryption algorithms must inherit.  Use the CipherFactory
	/// class to obtain an instance of this class.
{
public:
	typedef std::vector<unsigned char> ByteVec;

	enum Mode
		/// Cipher mode of operation. This mode determines how multiple blocks
		/// are connected; this is essential to improve security.
	{
		MODE_STREAM_CIPHER,	/// Stream cipher
		MODE_ECB,			/// Electronic codebook (plain concatenation)
		MODE_CBC,			/// Cipher block chaining (default)
		MODE_CFB,			/// Cipher feedback
		MODE_OFB			/// Output feedback
	};

	enum Encoding
		/// Transport encoding to use for encryptString() and decryptString().
	{
		ENC_NONE,		/// Plain binary output
		ENC_BASE64,		/// Base64-encoded output
		ENC_BINHEX		/// BinHex-encoded output
	};

	enum
	{
		DEFAULT_ITERATION_COUNT = 2000	/// Default iteration count to use with
										/// generateKey().  RSA security recommends
										/// an iteration count of at least 1000.
	};

	virtual ~Cipher();
		/// Destroys the Cipher.

	virtual const std::string& name() const = 0;
		/// Returns the name of the Cipher.

	virtual int keySize() const = 0;
		/// Returns the key size of the Cipher.

	virtual int blockSize() const = 0;
		/// Returns the block size of the Cipher.

	virtual int ivSize() const = 0;
		/// Returns the initialization vector (IV) size of the Cipher.

	virtual Mode mode() const = 0;
		/// Returns the Cipher's mode of operation.

	virtual const ByteVec& getKey() const = 0;
		/// Returns the key for the Cipher.

	virtual void setKey(const ByteVec& key) = 0;
		/// Sets the key for the Cipher.

	virtual const ByteVec& getIV() const = 0;
		/// Returns the initialization vector (IV) for the Cipher.

	virtual void setIV(const ByteVec& iv) = 0;
		/// Sets the initialization vector (IV) for the Cipher.

	virtual void generateKey();
		/// Generates a random key and IV.

	virtual void generateKey(const std::string& password, const std::string& salt = "", int iterationCount = DEFAULT_ITERATION_COUNT) = 0;
		/// Generates key and IV from a password and an optional salt string.
		/// RSA security recommends an iteration count of at least 1000.

	virtual CryptoTransform* createEncryptor() = 0;
		/// Creates an encrytor object to be used with a CryptoStream.

	virtual CryptoTransform* createDecryptor() = 0;
		/// Creates a decrytor object to be used with a CryptoStream.

	virtual std::string encryptString(const std::string& str, Encoding encoding = ENC_NONE);
		/// Directly encrypt a string and encode it using the given encoding.

	virtual std::string decryptString(const std::string& str, Encoding encoding = ENC_NONE);
		/// Directly decrypt a string that is encoded with the given encoding.

protected:
	Cipher();
		/// Creates a new Cipher object.

	void getRandomBytes(ByteVec& vector, std::size_t count);
		/// Fills the given vector with random bytes.

private:
	Cipher(const Cipher&);
	Cipher& operator = (const Cipher&);
};


} } // namespace Poco::Crypto


#endif // Crypto_Cipher_INCLUDED
