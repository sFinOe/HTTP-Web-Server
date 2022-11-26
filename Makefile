
### "make" for websevr 
### "make test" to compile tests 
### "test <testname>" to run a test

NAME = webserv
CC = c++ -std=c++98
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -g
RM = rm -rf
INCLUDE = ./includes/webserver.hpp ./includes/common.hpp
INCLUDES = -I./includes/ -I./tests/
TEST_INCLUDES = -I./includes/ -I./tests/
#wildcard in src/
SRCS = $(wildcard src/*.cpp src/parsing/*.cpp src/running/*.cpp)


# test srcs include all srcs except main.cpp
TEST_SRCS = $(wildcard tests/*.cpp) $(filter-out src/main.cpp, $(SRCS))
# TEST_SRCS+=$(filter-out ./src/main.cpp, $(SRCS))


PREFIX = ./obj/

OBJS = $(addprefix $(PREFIX), $(SRCS:.cpp=.o))

all: $(NAME)

$(PREFIX) :
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/src
	@mkdir -p $(PREFIX)/src/parsing
	@mkdir -p $(PREFIX)/src/running

$(NAME): $(PREFIX) $(OBJS) $(INCLUDE)
	$(CC) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(PREFIX)%.o: %.cpp $(INCLUDE)
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


test: $(TEST_SRCS) ./tests/tests.hpp
	$(CC) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_SRCS) -o test

clean:
	$(RM) $(OBJS) $(TEST_SRCS:.cpp=.o)
	@rm -rf $(PREFIX)

fclean: clean
	$(RM) $(NAME) test

re: fclean all

.PHONY: all clean fclean re

