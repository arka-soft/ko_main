#include <iostream>
#include "../syscore/syscore.h"

void dynamic_loop()
{
   SYSLOG_INFO("INFO" << std::endl);
   SYSLOG_WARN("WARN"<< std::endl);
   SYSLOG_ERROR("ERROR"<< std::endl);
   SYSLOG_DEBUG("DEBUG"<< std::endl);
   SYSLOG_SUCCESS("SUCCESS"<< std::endl);
   Sleep(1000);
}

