NAME = libft.a
CC = cc
AR = ar -rcs
CFLAGS = -Wall -Wextra -Werror
INCLUDE = libft.h
P1 = ft_strlen.c ft_memcpy.c ft_memmove.c ft_memset.c ft_isalpha.c ft_isdigit.c \
	 ft_isalnum.c ft_strlcat.c ft_strlcpy.c ft_isascii.c ft_isprint.c \
	 ft_bzero.c ft_strncmp.c ft_strnstr.c ft_atoi.c ft_strdup.c ft_toupper.c \
	 ft_tolower.c ft_strchr.c ft_strrchr.c ft_memchr.c ft_memcmp.c ft_calloc.c
P2 = ft_putstr_fd.c ft_strmapi.c ft_striteri.c ft_itoa.c ft_split.c ft_strtrim.c \
	ft_substr.c ft_putchar_fd.c ft_strjoin.c ft_putendl_fd.c ft_putnbr_fd.c
BONUS = ft_lstmap.c ft_lstiter.c ft_lstclear.c ft_lstdelone.c ft_lstadd_back.c ft_lstlast.c \
	ft_lstsize.c ft_lstadd_front.c ft_lstnew.c
RM = rm -f
OBJ = $(P1:.c=.o) $(P2:.c=.o)
B_OBJ = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ) 

clean:
	$(RM) $(B_OBJ) $(OBJ)

bonus: $(B_OBJ)
	$(AR) $(NAME) $(B_OBJ) 

fclean:
	$(RM) $(NAME) $(OBJ)

re: fclean all
