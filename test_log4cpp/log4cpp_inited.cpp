#include <stdio.h>
#include <stdlib.h>
#include "iostream"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"
#include "log4cpp/PropertyConfigurator.hh"

#define CODE_LOCATION __FILE__

// DEBUG < INFO < NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG

#define LOG_EMERG(__logstream)  __logstream << log4cpp::Priority::EMERG << CODE_LOCATION
#define LOG_ALERT(__logstream)  __logstream << log4cpp::Priority::ALERT << CODE_LOCATION
#define LOG_CRIT(__logstream)  __logstream << log4cpp::Priority::CRIT << CODE_LOCATION
#define LOG_ERROR(__logstream)  __logstream << log4cpp::Priority::ERROR << CODE_LOCATION
#define LOG_WARN(__logstream)  __logstream << log4cpp::Priority::WARN << CODE_LOCATION
#define LOG_NOTICE(__logstream)  __logstream << log4cpp::Priority::NOTICE << CODE_LOCATION
#define LOG_INFO(__logstream)  __logstream << log4cpp::Priority::INFO << CODE_LOCATION
#define LOG_DEBUG(__logstream)  __logstream << log4cpp::Priority::DEBUG << CODE_LOCATION

int main(int argc, char** argv) 
{
    log4cpp::Category &log_root = log4cpp::Category::getRoot();
    log4cpp::Category &log_sub1 = log4cpp::Category::getInstance( std::string("log_sub1") );
    log4cpp::Category &log_sub2 = log4cpp::Category::getInstance( std::string("log_sub1.log_sub2") );

    const char *file_log4cpp_init = "log4cpp_inited.log4cpp_properties";

    try
    {
        log4cpp::PropertyConfigurator::configure( file_log4cpp_init );
    }
    catch( log4cpp::ConfigureFailure &e )
    {
        std::cout 
            << e.what() 
            << " [log4cpp::ConfigureFailure catched] while reading " 
            << file_log4cpp_init 
            << std::endl;

        exit(1);
    }
 
    std::cout << " root priority = " << log_root.getPriority() << std::endl;
    std::cout << " log_sub1 priority = " << log_sub1.getPriority() << std::endl;
    std::cout << " log_sub2 priority = " << log_sub2.getPriority() << std::endl;

    //root

    log_root.error("root error");
    log_root.warn("root warn");

    //log_sub1

    log_sub1.error("log_sub1 error");
    log_sub1.warn("log_sub1 warn");
    log_sub1.info("%s%d", "i = ", 1122334455 );
    log_sub1.log( log4cpp::Priority::NOTICE, "reopen log" );
    log_sub1.info("log reopened");
    log_sub1.warn("could not reopen log");

    //log_sub2

    log_sub2.error("log_sub2 error");
    log_sub2.warn("log_sub2 warn");
    log_sub2.warnStream() << "streamed warn";
 
    log_sub2 
      << log4cpp::Priority::WARN 
      << "warn2" 
      << " warn3" 
      << log4cpp::eol 
      << " warn4"
      ;


}
