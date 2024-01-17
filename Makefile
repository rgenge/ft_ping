NAME = ping

CC = gcc

CFLAGS = -g -Wall -Wextra -Werror

SOURCE = ping.c utils.c

OBJ = $(SOURCE:.c=.o)

INC = ping.h

all: $(NAME) 

$(NAME) : ping.c $(INC)
	$(CC) $(CFLAGS) $(SOURCE) -o $(NAME)

clean:
	@rm -rf $(OBJ)
	
fclean: clean
	@rm -rf $(OBJ)
	@rm -rf $(NAME)

re: fclean all