NAME	= fractol
CC		= gcc
CFLAGS	= -Wextra -Wall -Werror

# Define the header location
# HEADERS = -I $(LIBFT) -I $(MLX) -I $(LIBMLX) -I $(FDF)
# HEADERS = -I $(MLX)

SRCS	= main.c
OBJS	= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

# mlx:
# 	$(MAKE) -C $(MLX)

clean:
	rm -f $(NAME)
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all, clean, fclean, re, mlx, libft