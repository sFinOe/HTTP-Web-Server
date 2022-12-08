
### "make" for websevr 
### "make test" to compile tests 
### "test <testname>" to run a test

NAME = webserv
BONUS = webserv_bonus
CC = c++ -std=c++11
CXXFLAGS= -Wall -Wextra -Werror -std=c++11 -g
RM = rm -rf
INCLUDE = ./includes/webserver.hpp ./includes/common.hpp
INCLUDE_BONUS = ./includes/webserver_bonus.hpp ./includes/common_bonus.hpp
INCLUDES = -I./includes/ -I./tests/
TEST_INCLUDES = -I./includes/ -I./tests/
#wildcard in src/
SRCS = $(wildcard src/parsing/*.cpp src/mandatory/*.cpp src/mandatory/running/*.cpp src/mandatory/running/request/*.cpp src/mandatory/running/response/*.cpp src/mandatory/running/cgi/*.cpp)
SRCS_BONUS = $(wildcard src/parsing/*.cpp src/bonus/*.cpp src/bonus/running/*.cpp src/bonus/running/request/*.cpp src/bonus/running/response/*.cpp src/bonus/running/cgi/*.cpp)


# test srcs include all srcs except main.cpp
TEST_SRCS = $(wildcard tests/*.cpp) $(filter-out src/main.cpp, $(SRCS))
# TEST_SRCS+=$(filter-out ./src/main.cpp, $(SRCS))


PREFIX = ./obj/

OBJS = $(addprefix $(PREFIX), $(SRCS:.cpp=.o))
OBJS_BONUS = $(addprefix $(PREFIX), $(SRCS_BONUS:.cpp=.o))

all: $(NAME)
bonus: $(NAME) $(BONUS)

$(PREFIX) :
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/src
	@mkdir -p $(PREFIX)/src/parsing
	@mkdir -p $(PREFIX)/src/mandatory/running
	@mkdir -p $(PREFIX)/src/mandatory/running/request
	@mkdir -p $(PREFIX)/src/mandatory/running/response
	@mkdir -p $(PREFIX)/src/mandatory/running/cgi
	@mkdir -p $(PREFIX)/src/bonus/running
	@mkdir -p $(PREFIX)/src/bonus/running/request
	@mkdir -p $(PREFIX)/src/bonus/running/response
	@mkdir -p $(PREFIX)/src/bonus/running/cgi

$(NAME): $(PREFIX) $(OBJS) $(INCLUDE)
	@$(CC) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@printf "\033[2K\r\033[0;32;1mTo Compile => ./webserv [/config/default.conf]\n\033[0m"

$(BONUS): $(PREFIX) $(OBJS_BONUS) $(INCLUDE_BONUS)
	@$(CC) $(CXXFLAGS) $(INCLUDES) $(OBJS_BONUS) -o $(BONUS)
	@printf "\033[2K\r\033[0;32;1mTo Compile => ./webserv_bonus [/config/default.conf]\n\033[0m"

$(PREFIX)%.o: %.cpp $(INCLUDE) $(INCLUDE_BONUS)
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@printf "\033[2K\r\033[0;32mCompiling $<\033[0m"

test: $(TEST_SRCS) ./tests/tests.hpp
	$(CC) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_SRCS) -o test

clean:
	@$(RM) $(OBJS) $(TEST_SRCS:.cpp=.o) $(OBJS_BONUS) 
	@rm -rf $(PREFIX)
	@printf "\033[2K\r\033[0;32mCleaned!\033[0m"

fclean: clean
	@$(RM) $(NAME) test $(BONUS)
	@printf "\033[2K\r\033[0;32mFile Cleaned!\033[0m"

re: fclean all

.PHONY: all clean fclean re

