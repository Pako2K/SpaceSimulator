#ifndef LOGGER_H
#define LOGGER_H

#ifdef SS_DEBUG
  #include <iostream>
  #define DEBUG_LOG(trace) std::cout << "DEBUG: " << trace << std::endl << std::endl
  #define ERROR_LOG(trace) std::cout << "***********************************************************************" << std::endl \
                                     << "ERROR: " << trace << std::endl \
                                     << "***********************************************************************" << std::endl << std::endl
#else
  #define DEBUG_LOG(trace)
  #define ERROR_LOG(trace)
#endif


#endif // LOGGER_H
