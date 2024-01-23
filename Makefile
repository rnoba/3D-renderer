NAME=wireframe_renderer
LIBFT_DIR=libft
LIBFT=$(LIBFT_DIR)/libft.a
GNL_DIR=get_next_line
GNL = $(GNL_DIR)/gnl.a
GNL_SRC = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
GNL_OBJ=$(GNL_SRC:%.c=%.o)
AR = ar -rcs
FILES=main.c
SRCS=$(FILES:%.c=$(SRC)/%.c)
MLX=MLX42
LIB_MLX=$(MLX)/build/libmlx42.a
FLAGS=-I$(MLX)/include -I$(GNL_DIR) -I$(LIBFT_DIR) -ldl -lglfw -pthread -lm
CFLAGS=-g3
SRC=main.c
CC=cc

all: $(NAME)

$(NAME): $(LIBFT) $(GNL)
	$(CC) -o $(NAME) $(SRC) $(LIB_MLX) $(GNL) $(LIBFT) $(FLAGS) $(CFLAGS)

$(GNL): $(GNL_OBJ)
	$(AR) $(GNL) $(GNL_OBJ)

$(LIBFT):
	make -C ./$(LIBFT_DIR)

clean:
	make clean -C ./$(LIBFT_DIR)
	rm $(GNL_OBJ)

fclean:
	make fclean -C ./$(LIBFT_DIR)
	rm -f $(GNL_OBJ)
	rm -f $(GNL)
	rm -f $(NAME)

re: fclean $(NAME)
