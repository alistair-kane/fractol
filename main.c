/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 19:05:05 by alistair          #+#    #+#             */
/*   Updated: 2022/03/26 03:05:46 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "fractol.h"

#include <stdio.h>
#include <unistd.h>

int colors[] = {0xFFFFFF, 0xFF00, 0xFF, 0xFF0000, 0x0};

int	destroy_exit(t_data *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	data->win_ptr = NULL;
	exit(0);
}

void	init_mandelbrot(t_data *data)
{
	data->x_min = -2;
	data->x_max = 1;
	data->y_min = -1;
	data->y_max = 1;
	data->zoom = 1.0;
}

double	interp(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

int	handle_keypress(int code, t_data *data)
{
	// if (code == XK_Escape)
	// printf("key: %d", code);
	if (code == 53)
		destroy_exit(data);
	if (code == 15)
		init_mandelbrot(data);
	
	printf("ratio: %f\n", data->zoom);
	//could set "pan size" var here
	if (code == 123)
	{
		data->x_min = data->x_min - (0.1 * data->zoom);
		data->x_max = data->x_max - (0.1 * data->zoom);
	}
	if (code == 124)
	{
		data->x_min = data->x_min + (0.1 * data->zoom);
		data->x_max = data->x_max + (0.1 * data->zoom);
	}
	if (code == 125)
	{
		data->y_min = data->y_min + (0.1 * data->zoom);
		data->y_max = data->y_max + (0.1 * data->zoom);
	}
	if (code == 126)
	{	
		data->y_min = data->y_min - (0.1 * data->zoom);
		data->y_max = data->y_max - (0.1 * data->zoom);
	}
	return (0);
}

void	zoom(t_data *data, int mouse_x, int mouse_y, int zoom_direction)
{
	double	mouse_re;
	double	mouse_im;
	double	interpolation;

	mouse_re = (double)mouse_x / (WINDOW_WIDTH / (data->x_max - data->x_min)) + data->x_min;
	mouse_im = (double)mouse_y / (WINDOW_HEIGHT / (data->y_max - data->y_min)) + data->y_min; 
	// zoom factor?
	double zoom_factor = 1.03;
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
	data->x_min = interp(mouse_re, data->x_min, interpolation);
	data->x_max = interp(mouse_re, data->x_max, interpolation);
	data->y_min = interp(mouse_im, data->y_min, interpolation);
	data->y_max = interp(mouse_im, data->y_max, interpolation);
}

int	handle_mouse(int code, int x, int y, t_data *data)
{
	if (code == 5)
	{
		// write(1, "up\n", 3);
		zoom(data, x, y, 0);
	}
	if (code == 4)
	{
		// write(1, "down\n", 5);
		zoom(data, x, y, 1);
	}
	return (0);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char    *pixel;

    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

int	create_trgb(t_rgb rgb)
{
	int r;
	int g;
	int b;
	
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	// free(rgb);
	return (0 << 24 | r << 16 | g << 8 | b);
}

// void	make_t_rgb(t_rgb *rgb, int r, int g, int b)
// {
// 	rgb->r = r;
// 	rgb->g = g;
// 	rgb->b = b;
// }

t_rgb	make_t_rgb(int r, int g, int b)
{
	t_rgb rgb;

	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	return(rgb);
}

int	hsv_to_rgb(t_hsv hsv)//, t_rgb rgb)
{
	t_rgb			rgb;
	t_vals			vals;
	unsigned char	region;
	unsigned char	remainder;

	// rgb = malloc(sizeof(t_rgb));
	// vals = malloc(sizeof(t_vals));
	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;
	vals.p = (hsv.v * (255 - hsv.s)) >> 8;
	vals.q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	vals.t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
	if (region == 0)
		rgb = make_t_rgb(hsv.v, vals.t, vals.p);
		// make_t_rgb(rgb, hsv.v, vals.t, vals.p);
	else if (region == 1)
		rgb = make_t_rgb(vals.q, hsv.v, vals.p);
		// make_t_rgb(rgb, vals.q, hsv.v, vals.p);
	else if (region == 2)
		rgb = make_t_rgb(vals.p, hsv.v, vals.t);
		// make_t_rgb(rgb, vals.p, hsv.v, vals.t);
	else if (region == 3)
		rgb = make_t_rgb(vals.p, vals.q, hsv.v);
		// make_t_rgb(rgb, vals.p, vals.q, hsv.v);
	else if (region == 4)
		rgb = make_t_rgb(vals.t, vals.p, hsv.v);
		// make_t_rgb(rgb, vals.t, vals.p, hsv.v);
	else
		rgb = make_t_rgb(hsv.v, vals.p, vals.q);
		// make_t_rgb(rgb, hsv.v, vals.p, vals.q);
	// free(vals);
	return (create_trgb(rgb));
}

double	mandelbrot(double x0, double y0)
{
	double	x;
	double	y;
	double	iteration;
	
	x = 0.0;
	y = 0.0;
	iteration = 0;
	while ((x * x + y * y <= 4) && iteration < MAX_ITER)
	{
		double xtmp = (x * x) - (y * y) + x0;
		y = 2 * y * x + y0;
		x = xtmp;
		iteration += 1;
	}
	if (iteration != MAX_ITER)
		iteration = iteration + 1 - log(log2(x * x + y * y));
	return (iteration);
}

void	plot_mandelbrot(t_data *data)
{
	int		y;
	int		x;
	double	n;
	t_hsv	hsv;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			double x0 = interp(data->x_min, data->x_max, x / (float)WINDOW_WIDTH);
			double y0 = interp(data->y_min, data->y_max, y / (float)WINDOW_HEIGHT);
			n = mandelbrot(x0, y0);
			// returns an amount of iterations which can then be assigned to a color
			hsv.h = ((255 * n) / MAX_ITER);
			hsv.s = 255;
			hsv.v = 255;
			if (n >= MAX_ITER)
				hsv.v = 0;
			// hsv.h = hsv.h / .10;
			img_pix_put(&(data->img), x++, y, hsv_to_rgb(hsv));
		}
		++y;
	}
}

int	render(t_data *data)
{
	/* if window has been destroyed, we don't want to put pixels */
	if (data->win_ptr == NULL)
		return (1);
	plot_mandelbrot(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

int	main(void)
{
	t_data	data;

	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	// could put the name of current fractal here
	init_mandelbrot(&data);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Fractol");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (MLX_ERROR);
	}
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);	
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);
	mlx_hook(data.win_ptr, 17, 0, &destroy_exit, &data);
	mlx_hook(data.win_ptr, 4, 0, &handle_mouse, &data);
	mlx_hook(data.win_ptr, 5, 0, &handle_mouse, &data);
	
	mlx_key_hook(data.win_ptr, &handle_keypress, &data);
	mlx_loop_hook(data.mlx_ptr, &render, &data);
	// mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keystroke, &data);
	mlx_mouse_hook(data.win_ptr, &handle_mouse, &data);
	mlx_hook(data.win_ptr, 17, 1L<<17, &destroy_exit, &data);
	mlx_loop(data.mlx_ptr);
	/* we will exit the loop if there's no window left, and execute this code */
	// mlx_destroy_display(data.mlx_ptr);
	mlx_destroy_window(data.mlx_ptr, data.win_ptr);
	free(data.mlx_ptr);
}	

// void	render_mandelbrot(t_img *img)
// {
// 	int	y;
// 	int	x;

// 	y = 0;
// 	while (y < WINDOW_HEIGHT)
// 	{
// 		x = 0;
// 		while (x < WINDOW_WIDTH)
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
// 	old_percent = x / (float)WINDOW_WIDTH;
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
// 	old_percent = y / (float)WINDOW_HEIGHT;
// 	new_y = interpolate(new_min, new_max, y / (float)WINDOW_HEIGHT);
// 	// new_y = ((new_max - new_min) * old_percent) + new_min;
// 	// new_y = (2.24 * old_percent) -(1.12);
// 	return (new_y);
// }