//
// ContainerTest.cpp
//
// Library: TestSuite
// Package: Container
// Module:  ContainerTest
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


#include "ContainerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Servlet/Container/WebContainer.h"
#include "Poco/Servlet/Container/WebApplication.h"
#include "Poco/Servlet/Container/WebServer.h"
#include "Poco/Servlet/Container/ServletContextImpl.h"
#include "Poco/Servlet/Container/ConfigImpl.h"
#include "Poco/Servlet/Ex/HttpServletDispatcher.h"
#include "Poco/Servlet/Container/SessionManagerImpl.h"
#include "Poco/Servlet/ServletException.h"
#include "Poco/Util/XMLConfiguration.h"
#include "Poco/Util/SystemConfiguration.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Path.h"
#include "Poco/Environment.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SocketAddress.h"
#include <algorithm>


using Poco::Util::XMLConfiguration;
using Poco::Util::SystemConfiguration;
using Poco::Util::LayeredConfiguration;
using Poco::AutoPtr;
using Poco::Path;
using Poco::Environment;
using Poco::Thread;
using Poco::Net::NetworkInterface;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::SocketAddress;
using Poco::InvalidArgumentException;
using Poco::ExistsException;
using Poco::NotFoundException;
using Poco::Servlet::Ex::HttpServletDispatcher;
using Poco::Servlet::Object;
using Poco::Servlet::Container::ServletContextImpl;
using Poco::Servlet::Container::ServletConfigImpl;
using Poco::Servlet::Container::FilterConfigImpl;
using Poco::Servlet::Container::WebServer;
using Poco::Servlet::Container::WebApplication;
using Poco::Servlet::Container::WebContainer;


ContainerTest::ContainerTest(const std::string& name): 
	CppUnit::TestCase(name), 
	_webAppDir("/webapps"), 
	_webSrvDir("/webservers")
{
  std::string base;

  try
  {
		base = Environment::get("POCO_BASE");
  }catch(NotFoundException&)
  {
		base = ".";
  }
  
  _rootDir = base + "/Servlet/runtime";
  _testServletPath = _rootDir + _webAppDir + "/ServletTest/";
}


ContainerTest::~ContainerTest()
{
}


void ContainerTest::testServletConfigImpl()
{
	HttpServletDispatcher servletDispatcher;
	ServletContextImpl scti(_testServletPath, servletDispatcher);
	ServletConfigImpl scfi(scti);
	
	scfi.setName("servlet");
	assert("servlet" == scfi.getServletName());

	scfi.setDisplayName("displayName");
	assert(scfi.getDisplayName() == "displayName");

	scfi.setClass("class");
	assert(scfi.getClass() == "class");

	scfi.setLibrary("library");
	assert(scfi.getLibrary() == "library");

	scfi.setInitParameter("initparam1", "value1");
	scfi.setInitParameter("initparam2", "value2");
	assert(scfi.getInitParameter("initparam1") == "value1");
	assert(scfi.getInitParameter("initparam2") == "value2");

	std::vector<std::string> names = scfi.getInitParameterNames();
	assert(names.size() == 2);
	assert(names[0] == "initparam1");
	assert(names[1] == "initparam2");
}


void ContainerTest::testFilterConfigImpl()
{
	HttpServletDispatcher servletDispatcher;
	ServletContextImpl scti(_testServletPath, servletDispatcher);
	FilterConfigImpl scfi(scti);
	
	scfi.setName("filter");
	assert("filter" == scfi.getFilterName());

	scfi.setDisplayName("displayName");
	assert(scfi.getDisplayName() == "displayName");

	scfi.setClass("class");
	assert(scfi.getClass() == "class");

	scfi.setLibrary("library");
	assert(scfi.getLibrary() == "library");

	scfi.setInitParameter("initparam1", "value1");
	scfi.setInitParameter("initparam2", "value2");
	assert(scfi.getInitParameter("initparam1") == "value1");
	assert(scfi.getInitParameter("initparam2") == "value2");

	std::vector<std::string> names = scfi.getInitParameterNames();
	assert(names.size() == 2);
	assert(names[0] == "initparam1");
	assert(names[1] == "initparam2");
}


void ContainerTest::testServletContextImpl()
{
	HttpServletDispatcher servletDispatcher;
	ServletContextImpl scti(_testServletPath, servletDispatcher);
	Object attr("myattr"), attr2("myattr2");
	scti.setAttribute(attr.getName(), &attr);
	assert(scti.getAttribute("myattr") != 0);
	scti.setAttribute(attr2.getName(), &attr2);
	std::vector<std::string> attrs = scti.getAttributeNames();
	assert(2 == attrs.size());
	assert(attr.getName() == attrs[0]);
	assert(attr2.getName() == attrs[1]);

	scti.removeAttribute("myattr");
	assert(0 == scti.getAttribute("myattr"));
	assert(0 == scti.getAttribute("notfound"));
	
	scti.setAttribute(attr2.getName(), 0);
	assert(0 == scti.getAttribute(attr2.getName()));

	scti.setInitParameter("initpar", "myinitpar");
	assert("myinitpar" == scti.getInitParameter("initpar"));
	assert("" == scti.getInitParameter("notfound"));
	assert("default" == scti.getInitParameter("notfound", "default"));

	static const std::string xmlFile = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<web-app>"
		"	<!-- General description of your web application -->"
		"	<display-name>Servlet Test Application</display-name>"
		"	<context-param>"
		"		<param-name>foo</param-name>"
		"		<param-value>bar</param-value>"
		"	</context-param>"
		"	<context-param>"
		"		<param-name>webmaster</param-name>"
		"		<param-value>AlFa</param-value>"
		"	</context-param>"
		"</web-app>";
		
	std::istringstream istr(xmlFile);	
	AutoPtr<XMLConfiguration> pXMLConf = new XMLConfiguration(istr);
	AutoPtr<SystemConfiguration> pSysConf = new SystemConfiguration;

	AutoPtr<LayeredConfiguration> pLayerConf = new LayeredConfiguration;
	pLayerConf->add(pXMLConf);
	pLayerConf->add(pSysConf);

	scti.setInitParameters(*pLayerConf.get());
	assert("bar" == scti.getInitParameter("foo"));
	assert("AlFa" == scti.getInitParameter("webmaster"));
	assert("" == scti.getInitParameter("notfound"));
	assert("default" == scti.getInitParameter("notfound", "default"));
}


void ContainerTest::testWebApplication()
{
	Path p(_testServletPath + "WEB-INF/web.xml", Path::PATH_UNIX);
	HttpServletDispatcher sd;
	WebApplication wa(_testServletPath, sd);

	wa.setInitParameter("initparam1", "value1");
	assert(wa.getInitParameter("initparam1") == "value1");

	try
	{
		assert(!wa.isServletClassRegistered("NOT_FOUND_Servlet_Class"));
		wa.createServlet("NOT_FOUND_Servlet_Class");
		fail("must throw");
	}
	catch(NotFoundException&)
	{
	}
	assert(!wa.isServletLoaded("NOT_FOUND_Servlet_Class"));

	assert(wa.isServletClassRegistered("TestServlet"));
	assertNotNull(wa.createServlet("TestServlet", "TestServlet"));
	assert(wa.isServletLoaded("TestServlet"));

	try
	{
		wa.createServlet("TestServlet", "TestServlet");
		fail("must throw");
	}
	catch(ExistsException&)
	{
	}

	assertNotNull(wa.getServlet("TestServlet"));
}


void ContainerTest::testWebServer()
{
	HttpServletDispatcher sd;
	WebApplication wa(_testServletPath, sd);

	WebServer ws(_rootDir + _webSrvDir + "/PocoServer/", sd);
	Thread t; t.start(ws);
	while(!ws.isRunning()) Thread::sleep(100);

	std::string iface = ws.interfaces() && ws.interfaces()->size() ? 
		*(ws.interfaces()->begin()) : NetworkInterface::list().begin()->address().toString() + ":80";
	SocketAddress sa(iface);
	HTTPClientSession cs(sa.host().toString(), sa.port());
	std::ostringstream uri;
	uri << "http://" << iface << "/ServletTest/Test/";

	HTTPRequest request("GET", uri.str());
	cs.sendRequest(request);

	std::string rbody;
	HTTPResponse response;
	cs.receiveResponse(response) >> rbody;

	assert(response.getStatus() == HTTPResponse::HTTP_OK);
	assert (rbody != "");

	ws.stop(); t.join();
}


void ContainerTest::testContainer()
{
	Path p(_rootDir, Path::PATH_UNIX);
	p.makeDirectory();
	std::string rootDir = p.toString();
	WebContainer c(rootDir, _webAppDir, _webSrvDir);
	Thread t; t.start(c);
	while(!c.isRunning()) Thread::sleep(300);
	c.stop();	t.join();
}


void ContainerTest::setUp()
{
}


void ContainerTest::tearDown()
{
}


CppUnit::Test* ContainerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ContainerTest");

	CppUnit_addTest(pSuite, ContainerTest, testServletContextImpl);
	CppUnit_addTest(pSuite, ContainerTest, testServletConfigImpl);
	CppUnit_addTest(pSuite, ContainerTest, testFilterConfigImpl);
	CppUnit_addTest(pSuite, ContainerTest, testContainer);	
	CppUnit_addTest(pSuite, ContainerTest, testWebApplication);
	CppUnit_addTest(pSuite, ContainerTest, testWebServer);
  
	return pSuite;
}
