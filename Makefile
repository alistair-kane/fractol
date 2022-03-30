NAME	= fractol
CC		= gcc
CFLAGS	= -Wextra -Wall -Werror -O3

# Define the header location
# HEADERS = -I $(LIBFT) -I $(MLX) -I $(LIBMLX) -I $(FDF)
# HEADERS = -I $(MLX)

SRCS	= fractals/mandelbrot.c fractals/burningship.c fractals/julia.c \
		src/utils_1.c src/utils_2.c src/plot_render.c src/colour.c src/input.c \
		src/complex_ops.c main.c 
OBJS	= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -Imlx -c $< -o $@


$(NAME): $(OBJS)
	$(CC) $(OBJS) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all, clean, fclean, re