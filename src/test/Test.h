#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "Log.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace Test{
    
	enum class Result : int{
		SUCCESS = 0, SKIPPED = 77, FAILURE = 99
	};
	
	class TestException : public std::runtime_error{
		private:
			Result result_;
		public:
			TestException(std::string message, Result result) : std::runtime_error(message), result_(result){};
		
			Result result() const{
				return result_;
			};
	};

        template<typename Arg> void print(std::ostream &output, Arg arg){
            output << arg;
        };
        
        template<typename Arg, typename... Args> void print(std::ostream &output, Arg arg, Args... args){
            output << arg;
            print<Args...>(output, args...);
        };        
        
	void assertTrue(std::string message, bool test){	
		if(!test){
			throw TestException(message, Result::FAILURE);
		}
	};
        
        template<typename... Args> void fail(Args... args){
            std::ostringstream buffer;
            Log::write(buffer, args...);
            throw TestException(buffer.str(), Result::FAILURE);
        };

	template<typename First, typename Second> void assertEquals(std::string message, First &&first, Second &&second){
		if(!(first == second)){
                    fail(message, " expected '", first, "', got '", second, "'");
                }
	};
	
}

std::ostream &operator<<(std::ostream &output, Test::Result result){
	return output << (int)result;
};

#define TEST_MAIN_WITH_ARGS(name,function)                                                                      \
	int main(int argCount, const char **args){								\
		try{                                                                                            \
                        std::cout << "TEST " << name << " started" << std::endl;                                \
			function(argCount, args);                                                  		\
                        std::cout << "TEST " << name << "success" << std::endl;                                 \
			return static_cast<int>(Test::Result::SUCCESS);						\
		}catch(Test::TestException &e){									\
			std::cout << "test failed with status code '" << e.result() << "' : " << std::endl;	\
			std::cout << e.what() << std::endl;							\
			return static_cast<int>(e.result());							\
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
			std::cout << "test failed with status code '" << e.result() << "' : " << std::endl;	\
			std::cout << e.what() << std::endl;							\
			return static_cast<int>(e.result());							\
		}catch(...){											\
			std::cout << "test failed with unexpected error" << std::endl;				\
			return static_cast<int>(Test::Result::FAILURE);						\
		}												\
	};

#endif
