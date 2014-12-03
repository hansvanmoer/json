#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "Log.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace Test {
    
    enum class Result : int {
        SUCCESS = 0, FAILURE= 1, SKIPPED = 77, FATAL_FAILURE = 99
    };

    class TestException : public std::runtime_error {
    private:
        Result result_;
    public:

        TestException(std::string message, Result result) : std::runtime_error(message), result_(result) {
        };

        Result result() const {
            return result_;
        };
    };
    
    template<typename Arg, typename... Args> void fail(Arg arg, Args... args){
        std::ostringstream buffer;
        Log::write(buffer,  arg, args...);
        Log::log("FAILURE: ", arg, args...);
        throw TestException(buffer.str(), Result::FAILURE);
    };
    
    void assertTrue(std::string message, Result result, bool test){
        if(!test){
            fail(message, result);
        }
    };
    
    void assertTrue(std::string message, bool test){
        assertTrue(message, Result::FAILURE, test);
    };
    
    template<typename T, typename U> void assertEquals(std::string message, T first, U second){
        assertTrue(message, first == second);
    };

}

std::ostream &operator<<(std::ostream &output, Test::Result result) {
    return output << (int) result;
};

#define TEST_MAIN_WITH_ARGS(name,function)                                                                      \
	int main(int argCount, const char **args){								\
                outputLogger = Logger{std::cout}                                                                \
                NullBuffer nullBuffer{};                                                                        \
                try{                                                                                            \
                        std::cout << "TEST " << name << " started" << std::endl;                                \
			function(argCount, args);                                                  		\
                        std::cout << "TEST " << name << "success" << std::endl;                                 \
			return static_cast<int>(Test::Result::SUCCESS);						\
		}catch(Test::TestException &e){                                                                 \
                        std::cout << "TEST FAILURE: " << e.what() << std::endl;                                 \							\
			std::cout << "test failed with status code '" << e.result() << "' : " << std::endl;	\
			std::cout << e.what() << std::endl;							\
			return static_cast<int>(e.result());							\
                }catch(std::exception &e){                                                                       \
			std::cout << "test failed with unexpected error" << std::endl;                          \
                        std::cout << "message: '" << e.what() << "'" << std::endl;                              \
			return static_cast<int>(Test::Result::FAILURE);						\
		}catch(...){											\
			std::cout << "test failed with unexpected error" << std::endl;				\
			return static_cast<int>(Test::Result::FAILURE);						\
		}												\
	};

#define TEST_MAIN(name,function)                                                                                \
	int main(int argCount, const char **args){								\
		try{												\
                        std::cout << "TEST " << name << " started" << std::endl;                                \
			function();										\
                        std::cout << "TEST " << name << "success" << std::endl;                                 \
			return static_cast<int>(Test::Result::SUCCESS);						\
		}catch(Test::TestException &e){									\
                        std::cout << "TEST FAILURE: " << e.what() << std::endl;                                \
			std::cout << "test failed with status code '" << e.result() << "' : " << std::endl;	\
			std::cout << e.what() << std::endl;							\
			return static_cast<int>(e.result());							\
                }catch(std::exception &e){                                                                       \
			std::cout << "test failed with unexpected error" << std::endl;                          \
                        std::cout << "message: '" << e.what() << "'" << std::endl;                              \
			return static_cast<int>(Test::Result::FAILURE);						\
                }catch(...){											\
			std::cout << "test failed with unexpected error" << std::endl;				\
			return static_cast<int>(Test::Result::FAILURE);						\
		}												\
	};

#endif
