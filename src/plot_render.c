/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plot_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 19:41:17 by alkane            #+#    #+#             */
/*   Updated: 2022/04/01 17:53:13 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

static void	plot_mandelbrot(t_data *data, int burn)
{
	int		y;
	int		x;
	double	n;
	t_hsv	hsv;
	t_cplx	cplx_z;

	y = -1;
	while (++y < W_HEIGHT)
	{
		x = 0;
		while (x < W_WIDTH)
		{
			cplx_z.real = (data->re_min + (x / (double)W_WIDTH) \
				* (data->re_max - data->re_min));
			cplx_z.imag = (data->im_min + (y / (double)W_HEIGHT) \
				* (data->im_max - data->im_min));
			n = mandelbrot(cplx_z, burn, data->iterations);
			hsv.h = interp(data->hsv_min, data->hsv_max, n) / data->iterations;
			hsv.s = 255;
			hsv.v = 255;
			if (n >= data->iterations)
				hsv.v = 0;
			img_pix_put(&(data->img), x++, y, hsv_to_rgb(hsv));
		}
	}
}

static void	plot_julia(t_data *data)
{
	int		y;
	int		x;
	double	n;
	t_hsv	hsv;
	t_cplx	cplx;

	y = -1;
	while (++y < W_HEIGHT)
	{
		x = 0;
		while (x < W_WIDTH)
		{
			cplx.real = (data->re_min + (x / (double)W_WIDTH) \
				* (data->re_max - data->re_min));
			cplx.imag = (data->im_min + (y / (double)W_HEIGHT) \
				* (data->im_max - data->im_min));
			n = julia(data->con_real, data->con_imag, cplx, data->iterations);
			hsv.h = interp(data->hsv_min, data->hsv_max, n) / data->iterations;
			hsv.s = 255;
			hsv.v = 255;
			if (n >= data->iterations)
				hsv.v = 0;
			img_pix_put(&(data->img), x++, y, hsv_to_rgb(hsv));
		}
	}
}

static void	var_display(t_data data)
{
	char	*max_iters;
	char	*min_hsv;
	char	*max_hsv;

	max_iters = ft_strjoin("Max Iterations: ", ft_itoa(data.iterations));
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 30, 0xFFFFFF, \
		max_iters);
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 50, 0xFFFFFF, \
		"- : F5 | + : F6");
	min_hsv = ft_strjoin("Min HSV Val: ", ft_itoa(data.hsv_min));
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 100, 0xFFFFFF, \
		min_hsv);
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 120, 0xFFFFFF, \
		"- : 1 | + : 2");
	max_hsv = ft_strjoin("Max HSV Val: ", ft_itoa(data.hsv_max));
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 140, 0xFFFFFF, \
		max_hsv);
	mlx_string_put(data.mlx_ptr, data.win_ptr, W_WIDTH - 200, 160, 0xFFFFFF, \
		"- : 3 | + : 4");
	free(max_iters);
	free(min_hsv);
	free(max_hsv);
}

int	render(t_data *data)
{
	if (data->win_ptr == NULL)
		return (1);
	if (!ft_strncmp(data->name, "Mandelbrot", 10))
		plot_mandelbrot(data, 0);
	if (!ft_strncmp(data->name, "Burning Ship", 12))
		plot_mandelbrot(data, 1);
	if (!ft_strncmp(data->name, "Julia", 5))
		plot_julia(data);
	if (data->trip_toggle < 1)
		trip_control(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, \
		0, 0);
	var_display(*data);
	mlx_string_put(data->mlx_ptr, data->win_ptr, 300, W_HEIGHT - 50, 0xFFFFFF, \
		"Toggle Trippy: T");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 500, W_HEIGHT - 50, 0xFFFFFF, \
		"Reset Key: R");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 700, W_HEIGHT - 50, 0xFFFFFF, \
		"Zoom: Mousewheel");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 900, W_HEIGHT - 50, 0xFFFFFF, \
		"Pan: Arrow Keys");
	return (0);
}
