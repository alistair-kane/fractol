/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 19:05:05 by alistair          #+#    #+#             */
/*   Updated: 2022/03/27 19:36:22 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "fractol.h"

#include <stdio.h>
#include <unistd.h>

const char	*g_supported[] = {"Mandelbrot", "Burning Ship", "Julia 1", \
	"Julia 2", "Julia 3", "Julia 4"};

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

double	interp(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

void	arrow_keys(t_data *data, int code)
{
	if (code == 123)
	{
		data->re_min = data->re_min - (0.1 * data->zoom);
		data->re_max = data->re_max - (0.1 * data->zoom);
	}
	if (code == 124)
	{
		data->re_min = data->re_min + (0.1 * data->zoom);
		data->re_max = data->re_max + (0.1 * data->zoom);
	}
	if (code == 125)
	{
		data->im_min = data->im_min + (0.1 * data->zoom);
		data->im_max = data->im_max + (0.1 * data->zoom);
	}
	if (code == 126)
	{	
		data->im_min = data->im_min - (0.1 * data->zoom);
		data->im_max = data->im_max - (0.1 * data->zoom);
	}
}

int	handle_keypress(int code, t_data *data)
{
	if (code == 15)
		init(data, data->name);
	if (code == 53)
		destroy_exit(data);
	if (code == 96)
		data->iterations = data->iterations - 20;
	if (code == 97)
		data->iterations = data->iterations + 20;
	if (code >= 123 && code <= 126)
		arrow_keys(data, code);
	return (0);
}

void	zoom(t_data *data, int mouse_x, int mouse_y, int zoom_direction)
{
	double	mouse_re;
	double	mouse_im;
	double	interpolation;
	double	zoom_factor;

	mouse_re = (double)mouse_x / (W_WIDTH / (data->re_max - data->re_min)) \
		+ data->re_min;
	mouse_im = (double)mouse_y / (W_HEIGHT / (data->im_max - data->im_min)) \
		+ data->im_min;
	zoom_factor = 1.02;
	if (!zoom_direction)
	{
		interpolation = zoom_factor / 1.0;
		data->zoom = zoom_factor / data->zoom;
	}
	else
	{
		interpolation = 1.0 / zoom_factor;
		data->zoom = data->zoom / zoom_factor;
	}
	data->re_min = interp(mouse_re, data->re_min, interpolation);
	data->re_max = interp(mouse_re, data->re_max, interpolation);
	data->im_min = interp(mouse_im, data->im_min, interpolation);
	data->im_max = interp(mouse_im, data->im_max, interpolation);
}

int	handle_mouse(int code, int x, int y, t_data *data)
{
	if (code == 5)
		zoom(data, x, y, 0);
	if (code == 4)
		zoom(data, x, y, 1);
	return (0);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

int	create_trgb(t_rgb rgb)
{
	int	r;
	int	g;
	int	b;

	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	return (0 << 24 | r << 16 | g << 8 | b);
}

t_rgb	make_t_rgb(int r, int g, int b)
{
	t_rgb	rgb;

	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	return (rgb);
}

int	hsv_to_rgb(t_hsv hsv)
{
	t_rgb			rgb;
	t_vals			vals;
	unsigned char	region;
	unsigned char	remainder;

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;
	vals.p = (hsv.v * (255 - hsv.s)) >> 8;
	vals.q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	vals.t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
	if (region == 0)
		rgb = make_t_rgb(hsv.v, vals.t, vals.p);
	else if (region == 1)
		rgb = make_t_rgb(vals.q, hsv.v, vals.p);
	else if (region == 2)
		rgb = make_t_rgb(vals.p, hsv.v, vals.t);
	else if (region == 3)
		rgb = make_t_rgb(vals.p, vals.q, hsv.v);
	else if (region == 4)
		rgb = make_t_rgb(vals.t, vals.p, hsv.v);
	else
		rgb = make_t_rgb(hsv.v, vals.p, vals.q);
	return (create_trgb(rgb));
}

// trippy bonus: hsv.h = hsv.h / .10; (place before pix_put)
void	plot_mandelbrot(t_data *data, int burn)
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
			hsv.h = ((255 * n) / data->iterations);
			hsv.s = 255;
			hsv.v = 255;
			if (n >= data->iterations)
				hsv.v = 0;
			img_pix_put(&(data->img), x++, y, hsv_to_rgb(hsv));
		}
	}
}

void	plot_julia(t_data *data)
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
			hsv.h = ((255 * n) / data->iterations);
			hsv.s = 255;
			hsv.v = 255;
			if (n >= data->iterations)
				hsv.v = 0;
			img_pix_put(&(data->img), x++, y, hsv_to_rgb(hsv));
		}
	}
}

int	render(t_data *data)
{
	char	*max_iters;

	if (data->win_ptr == NULL)
		return (1);
	if (!ft_strncmp(data->name, "Mandelbrot", 10))
		plot_mandelbrot(data, 0);
	if (!ft_strncmp(data->name, "Burning Ship", 12))
		plot_mandelbrot(data, 1);
	if (!ft_strncmp(data->name, "Julia", 5))
		plot_julia(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, \
		0, 0);
	max_iters = ft_strjoin("Max Iterations: ", ft_itoa(data->iterations));
	mlx_string_put(data->mlx_ptr, data->win_ptr, W_WIDTH - 200, 30, 0xFFFFFF, \
		max_iters);
	mlx_string_put(data->mlx_ptr, data->win_ptr, W_WIDTH - 200, 50, 0xFFFFFF, \
		"- : F5 | + : F6");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 400, W_HEIGHT - 50, 0xFFFFFF, \
		"Reset Key: R");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 600, W_HEIGHT - 50, 0xFFFFFF, \
		"Zoom: Mousewheel");
	mlx_string_put(data->mlx_ptr, data->win_ptr, 800, W_HEIGHT - 50, 0xFFFFFF, \
		"Pan: Arrow Keys");
	free(max_iters);
	return (0);
}

void	error_return(t_data *data)
{
	int	i;

	i = -1;
	ft_putstr_fd("Error: Available inputs:\n", 1);
	while (++i < 6)
	{
		ft_putstr_fd("\"", 1);
		ft_putstr_fd(g_supported[i], 1);
		ft_putstr_fd("\"\n", 2);
	}
	free(data->mlx_ptr);
	exit(EXIT_SUCCESS);
}

void	check_input(t_data *data, char *name)
{
	int	i;
	int	no_success;

	i = -1;
	no_success = 1;
	while (++i < 6)
	{
		if (!ft_strncmp(name, g_supported[i], ft_strlen(g_supported[i])) \
			&& ft_strlen(name) == ft_strlen(g_supported[i]))
		{
			no_success = init(data, name);
			break ;
		}
	}
	if (no_success)
		error_return(data);
}

int	main(int argc, char **argv)
{
	t_data	data;

	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	if (argc == 2)
		check_input(&data, argv[1]);
	else
		error_return(&data);
	data.win_ptr = mlx_new_window(data.mlx_ptr, W_WIDTH, W_HEIGHT, data.name);
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (MLX_ERROR);
	}
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

// void	render_mandelbrot(t_img *img)
// {
// 	int	y;
// 	int	x;

// 	y = 0;
// 	while (y < W_HEIGHT)
// 	{
// 		x = 0;
// 		while (x < W_WIDTH)
// 		{
// 			float x_s = scale_x(x);
// 			float y_s = scale_y(y);
// 			float ix = 0.0;
// 			float iy = 0.0;
// 			int iteration = 0;
// 			int max_iteration = 1000;
			
// 			while ((ix*ix + iy*iy <= 4) && iteration < max_iteration)
// 			{
// 				float xtmp = (ix*ix) - (iy*iy) + x_s;
// 				iy = 2 * iy * ix + y_s;
// 				ix = xtmp;
// 				iteration += 1;
// 			}
// 			// printf("iterations: %d\n", iteration);

// 			// returns an amount of iterations which can then be assigned to a color
// 			int color = generate_color(iteration);
// 			img_pix_put(img, x++, y, color);
// 		}
// 		++y;
// 	}
// }

// x is real part
// double	scale_x(int x)
// {
// 	// https://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
// 	double	old_percent;
// 	double	new_x;
// 	double	new_min;
// 	double	new_max;
	
// 	new_min = 0.750222 * -1;
// 	new_max	= 0.749191 * -1;
// 	// new_min = 0.34853774148008254 * -1;
// 	// new_max = 0.34831493420245574 * -1;
// 	// new_min = -2;
// 	// new_max = 2;
// 	old_percent = x / (float)W_WIDTH;
// 	new_x = interpolate(new_min, new_max, old_percent);// * 1 / 10;
// 	// new_x = ((new_max - new_min) * old_percent) + new_min;
// 	// new_x = (2.47 * old_percent) -2.00;
// 	return (new_x);
// }

// y is imaginary??
// double	scale_y(int y)
// {
// 	double	old_percent;
// 	double	new_y;
// 	double	new_min;
// 	double	new_max;
	
// 	new_min = 0.031161;
// 	new_max	= 0.031752;
// 	// new_min = 0.6065922085831237 * -1;
// 	// new_max = 0.606486596104741 * -1;
// 	// new_min = -1;
// 	// new_max = 1;
// 	old_percent = y / (float)W_HEIGHT;
// 	new_y = interpolate(new_min, new_max, y / (float)W_HEIGHT);
// 	// new_y = ((new_max - new_min) * old_percent) + new_min;
// 	// new_y = (2.24 * old_percent) -(1.12);
// 	return (new_y);
// }

// t_cplx	sub_comp(t_cplx e1, t_cplx e2)
// {
// 	t_cplx e3;

// 	e3.real = fabs(e1.real - e2.real);
// 	e3.imag = fabs(e1.imag - e2.imag);
// 	return (e3);
// }

// double	mandelbrot(t_cplx *cplx)
// {
// 	double	x;
// 	double	y;
// 	double	iteration;
// 	t_cplx *z;
// 	z = malloc(sizeof(t_cplx));
// 	z->real = 0.0;
// 	z->imag = 0.0;
// 	iteration = 0;
// 	x = 0.0;
// 	y = 0.0;
// 	while (abs_comp(z) <= 2 && iteration < MAX_ITER)
// 	// {
// 	// 	// t_cplx temp = ;
		
// 	// 	z = add_comp(mul_comp(z, z), cplx);
// 	// while ((pow(x, 2) + pow(y, 2) <= 4) && iteration < MAX_ITER)
// 	{
// 		// printf("without complex: %f\n", (pow(x, 2) + pow(y, 2)));
// 		// printf("with complex   : %f\n\n",abs_comp(z));
		
		
// 		// z = mul_comp(z, z);
// 		z = add_comp(cplx, mul_comp(z, z));
		
// 		// double xtmp = (x * x) - (y * y) + x0;
// 		// y = 2 * y * x + y0;
// 		// x = xtmp;
// 		iteration += 1;
// 	}
// 	// free(z);
// 	if (iteration == MAX_ITER)
// 		return(MAX_ITER);
// 	return (iteration + 1 - log(log2(abs_comp(z))));
// 	// return (iteration + 1 - log(log2(x * x + y * y)));
// }