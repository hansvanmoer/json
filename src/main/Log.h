/* 
 * File:   Log.h
 * Author: hans
 *
 * Created on 29 October 2014, 07:27
 */

#ifndef LOG_H
#define	LOG_H

#include <iostream>

namespace Log {

    namespace _Private {

        using Yes = char;

        struct No {
            char value[2];
        };

        class Wrapper {
        public:
            template<typename T> Wrapper(T);
        };

    }
    
    _Private::No operator<<(std::ostream &, _Private::Wrapper);

    template<typename Char, typename CharTraits, typename Type> class isOutputStreamInsertable {
    public:
        using OutputStream = std::basic_ostream<Char, CharTraits>;

    private:

        static _Private::Yes test(OutputStream &);

        static _Private::No test(_Private::No);

        static OutputStream *testOutput_;

        static const Type *testValue_;

    public:

        static const bool value = sizeof (test(*testOutput_ << *testValue_)) == sizeof (_Private::Yes);
    };

    namespace _Private {

        template<typename Char, typename CharTraits, bool insertable> class ValueWriterHelper {
        public:

            template<typename Type> static void write(std::basic_ostream<Char, CharTraits> &output, const Type &) {
                output << "<value cannot be serialized>";
            };

        };

        template<typename Char, typename CharTraits> class ValueWriterHelper<Char, CharTraits, true> {
        public:

            template<typename Type> static void write(std::basic_ostream<Char, CharTraits> &output, const Type &value) {
                output << value;
            };
        };

        template<typename Char, typename CharTraits, typename... Types> class ValueWriter;

        template<typename Char, typename CharTraits, typename Type, typename... Types> class ValueWriter<Char, CharTraits, Type, Types...> {
        public:

            using OutputStream = std::basic_ostream<Char, CharTraits>;

        public:

            static void write(OutputStream &output, Type value, Types... values) {
                _Private::ValueWriterHelper<Char, CharTraits, isOutputStreamInsertable<Char, CharTraits, Type>::value>::write(output, value);
                ValueWriter<Char, CharTraits, Types...>::write(output, values...);
            };

        };

        template<typename Char, typename CharTraits, typename Type> class ValueWriter<Char, CharTraits, Type> {
        public:

            using OutputStream = std::basic_ostream<Char, CharTraits>;

        public:

            static void write(OutputStream &output, Type value) {
                ValueWriterHelper<Char, CharTraits, isOutputStreamInsertable<Char, CharTraits, Type>::value>::write(output, value);
            };

        };

        template<typename Char, typename CharTraits, typename Arg, typename... Args> void writeLog(std::basic_ostream<Char, CharTraits> &output, Arg arg, Args... args) {
            ValueWriter<Char, CharTraits, Arg>::write(output, arg);
        };
    }

    template<typename Arg, typename... Args> void log(Arg arg, Args... args){
        _Private::ValueWriter<char, std::char_traits<char>,Arg, Args...>::write(std::cout, arg, args...);
        std::cout << std::endl;
    };
    
    template<typename Char, typename CharTraits, typename Arg, typename... Args> void write(std::basic_ostream<Char, CharTraits> &output, Arg arg, Args... args){
        _Private::ValueWriter<Char, CharTraits,Arg, Args...>::write(output, arg, args...);
        output << std::endl;
    };
    
}

#endif	/* LOG_H */

