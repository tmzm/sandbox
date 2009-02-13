//
// OptionSetTest.cpp
//
// $Id: //poco/svn/Util/testsuite/src/OptionSetTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "OptionSetTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionException.h"


using Poco::Util::OptionSet;
using Poco::Util::Option;


OptionSetTest::OptionSetTest(const std::string& name): CppUnit::TestCase(name)
{
}


OptionSetTest::~OptionSetTest()
{
}


void OptionSetTest::testOptionSet()
{
	OptionSet set;
	set.addOption(
		Option("helper", "H", "start helper")
			.required(false)
			.repeatable(false));
	set.addOption(
		Option("help", "h", "print help text")
			.required(false)
			.repeatable(false));
	set.addOption(
		Option("include-dir", "I", "specify a search path for locating header files")
			.required(false)
			.repeatable(true)
			.argument("path"));
	set.addOption(
		Option("library-dir", "L", "specify a search path for locating library files")
			.required(false)
			.repeatable(true)
			.argument("path"));
	set.addOption(
		Option("insert", "it", "insert something")
			.required(false)
			.repeatable(true)
			.argument("path"));
	set.addOption(
		Option("item", "", "insert something")
			.required(false)
			.repeatable(true)
			.argument("path"));
	set.addOption(
		Option("include", "J", "specify a search path for locating header files")
			.required(false)
			.repeatable(true)
			.argument("path"));
	
	assert (set.hasOption("include", false));
	assert (set.hasOption("I", true));
	assert (set.hasOption("Include", true));
	assert (set.hasOption("insert", false));
	assert (set.hasOption("it", true));
	assert (set.hasOption("Insert", false));
	assert (set.hasOption("item", false));
	assert (!set.hasOption("i", false));
	assert (!set.hasOption("in", false));
	
	assert (set.hasOption("help"));
	assert (set.hasOption("h", true));
	assert (set.hasOption("helper"));
	assert (set.hasOption("H", true));
	
	const Option& opt1 = set.getOption("include");
	assert (opt1.fullName() == "include");

	const Option& opt2 = set.getOption("item");
	assert (opt2.fullName() == "item");

	const Option& opt3 = set.getOption("I", true);
	assert (opt3.fullName() == "include-dir");

	const Option& opt4 = set.getOption("include-d");
	assert (opt4.fullName() == "include-dir");

	const Option& opt5 = set.getOption("help");
	assert (opt5.fullName() == "help");

	const Option& opt6 = set.getOption("helpe");
	assert (opt6.fullName() == "helper");
	
	try
	{
		set.getOption("in");
		fail("ambiguous - must throw");
	}
	catch (Poco::Util::AmbiguousOptionException&)
	{
	}

	try
	{
		set.getOption("he");
		fail("ambiguous - must throw");
	}
	catch (Poco::Util::AmbiguousOptionException&)
	{
	}

	try
	{
		set.getOption("i");
		fail("ambiguous - must throw");
	}
	catch (Poco::Util::AmbiguousOptionException&)
	{
	}
}


void OptionSetTest::setUp()
{
}


void OptionSetTest::tearDown()
{
}


CppUnit::Test* OptionSetTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OptionSetTest");

	CppUnit_addTest(pSuite, OptionSetTest, testOptionSet);

	return pSuite;
}
