/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 19:05:05 by alistair          #+#    #+#             */
/*   Updated: 2022/03/27 22:53:59 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	destroy_exit(t_data *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	data->win_ptr = NULL;
	exit(0);
}

int	init(t_data *data, char *input)
{
	data->name = input;
	if (!ft_strncmp(data->name, "Mandelbrot", 10))
		init_mandelbrot(data);
	if (!ft_strncmp(data->name, "Burning Ship", 12))
		init_burningship(data);
	if (!ft_strncmp(data->name, "Julia 1", 7))
		init_julia_1(data);
	if (!ft_strncmp(data->name, "Julia 2", 7))
		init_julia_2(data);
	if (!ft_strncmp(data->name, "Julia 3", 7))
		init_julia_3(data);
	if (!ft_strncmp(data->name, "Julia 4", 7))
		init_julia_4(data);
	data->iterations = DEFAULT_ITER;
	data->zoom = 1.0;
	return (0);
}

void	error_return(t_data *data, const char **supported)
{
	int	i;

	i = -1;
	ft_putstr_fd("Error: Available inputs:\n", 1);
	while (++i < 6)
	{
		ft_putstr_fd("\"", 1);
		ft_putstr_fd(supported[i], 1);
		ft_putstr_fd("\"\n", 2);
	}
	free(data->mlx_ptr);
	exit(EXIT_SUCCESS);
}

void	check_input(t_data *data, char *name, const char **supported)
{
	int	i;
	int	no_success;

	i = -1;
	no_success = 1;
	while (++i < 6)
	{
		if (!ft_strncmp(name, supported[i], ft_strlen(supported[i])) \
			&& ft_strlen(name) == ft_strlen(supported[i]))
		{
			no_success = init(data, name);
			break ;
		}
	}
	if (no_success)
		error_return(data, supported);
}

int	main(int argc, char **argv)
{
	t_data		data;
	const char	*supported[] = {"Mandelbrot", "Burning Ship", "Julia 1", \
		"Julia 2", "Julia 3", "Julia 4"};

	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	if (argc == 2)
		check_input(&data, argv[1], supported);
	else
		error_return(&data, supported);
	data.win_ptr = mlx_new_window(data.mlx_ptr, W_WIDTH, W_HEIGHT, data.name);
	if (data.win_ptr == NULL)
		destroy_exit(&data);
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, W_WIDTH, W_HEIGHT);
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, \
		&data.img.line_len, &data.img.endian);
	mlx_hook(data.win_ptr, 17, 0, &destroy_exit, &data);
	mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_mouse_hook(data.win_ptr, &handle_mouse, &data);
	mlx_key_hook(data.win_ptr, &handle_keypress, &data);
	mlx_loop(data.mlx_ptr);
	mlx_destroy_window(data.mlx_ptr, data.win_ptr);
}
