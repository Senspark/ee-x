/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */



#ifndef __CCSoomlaUtils_H_
#define __CCSoomlaUtils_H_


#include "CCError.h"

#ifndef SOOMLA_DEBUG
#define SOOMLA_DEBUG false
#endif

namespace soomla {
	/** 
     @class CCSoomlaUtils
     @brief This class handles printing of error and debug messages.

     This class handles printing of error and debug messages.
	 */
    class CCSoomlaUtils {
    public:
		/**
         Prints a debug message.
         @param tag The name of the class whose instance called this function.
         @param message The debug message to output to log.
		 */
        static void logDebug(const std::string& tag, const std::string& message);

		/**
         Prints an error message.
         @param tag The name of the class whose instance called this function.
         @param message The error message to output to log.
		*/
        static void logError(const std::string& tag, const std::string& message);

		/**
         Prints an exception message.
         @param tag The name of the class whose instance called this function.
         @param error A CCSoomlaError from which to extract the message.
		*/
        static void logException(const std::string& tag, CCError *error);
        
        /**
         Checks if a string is either NULL or empty string
         @param str The string to check
         @return true if the string is null or empty, false otherwise
         */
        static bool isEmpty(const std::string& str);
    };
};

#endif //__CCSoomlaUtils_H_
