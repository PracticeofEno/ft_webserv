NAME			= webserv

CC				= c++
CFLAGS			= -Wall -Werror -Wextra -std=c++98

SRCS			= main.cpp \
					Server.cpp

OBJS			= $(SRCS:.cpp=.o)

$(NAME)			: $(OBJS)
					$(CC) $(CFLAGS) $(OBJS) -o $@

%.o				: %.cpp
					$(CC) $(CFLAGS) -c $< -o $@

all				: $(NAME)

clean			:
					rm -rf $(OBJS)

fclean			:
					rm -rf $(NAME) $(OBJS)

re				: fclean all

.PHONY			: all clean fclean re bonus
