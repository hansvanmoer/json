/* 
 * File:   TestLogger.h
 * Author: hans
 *
 * Created on 13 November 2014, 19:56
 */

#ifndef TESTLOGGER_H
#define	TESTLOGGER_H

#include <iostream>

namespace Test {

    class LogOutputWrapper {
    public:

        template<typename T> LogOutputWrapper(const T &value) {
        };
    };

    using Writable = char;

    struct NotWritable {
        char data[2];
    };

    NotWritable operator<<(std::ostream &output, LogOutputWrapper wrapper);

    template<typename T> class IsOutputStreamInsertable {
    private:

        Writable test(std::ostream &);

        NotWritable test(NotWritable);

        static std::ostream *stream;

        static const T *data;

    public:

        static const bool value = sizeof (test((*stream) << (*data))) == sizeof (Writable);

    };
    
    

}

#endif	/* TESTLOGGER_H */

