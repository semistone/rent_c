#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "../src/DB.h"

using namespace CppUnit;
using namespace std;
using namespace dfor;

//-----------------------------------------------------------------------------

class TestDB: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestDB);
    CPPUNIT_TEST(testCreate);
    CPPUNIT_TEST(testQuery);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testCreate(void);
    void testQuery(void);

private:
    DB *test;
};

//-----------------------------------------------------------------------------

void
TestDB::testCreate(void)
{
    test->create();
}

void 
TestDB::testQuery(void)
{
    try{
        HostRecord host = HostRecord(std::string("localhost"), std::string("127.0.0.1"), 1, (long)0);
        int ret = test->insert(host);
        //printf("insert result %d\n", ret);
        std::string ip = test->query("localhost");
        ret = test->updateCount(host.name, host.ip);
        //printf(ip.c_str());
    }catch(std::runtime_error& e){
        printf(e.what());    
    }
}

void TestDB::setUp(void)
{
    test = new DB(std::string("example.db"));
}

void TestDB::tearDown(void)
{
    delete test;
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(TestDB);

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("cppTestBasicMathResults.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
