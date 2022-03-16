/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 19:05:05 by alistair          #+#    #+#             */
/*   Updated: 2022/03/16 14:06:39 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#include <stdio.h>

int colors[] = {0xFFFFFF, 0xFF00, 0xFF, 0xFF0000, 0x0};

int	destroy_exit(t_data *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	data->win_ptr = NULL;
	exit(0);
}

int	handle_keypress(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		destroy_exit(data);
	// handle the other presses here?
	return (0);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char    *pixel;

    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

float	scale_x(int x)
{
	// https://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
	float	old_percent;
	float	new_val;
	
	old_percent = x / (float)WINDOW_WIDTH;
	// printf("old %%: %f",old_percent);
	new_val = (2.47 * old_percent) -2.00;
	return (new_val);
}

float	scale_y(int y)
{
	float	old_percent;
	float	new_val;

	old_percent = y / (float)WINDOW_HEIGHT;
	new_val = (2.24 * old_percent) -(1.12);
	return (new_val);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	make_t_rgb(t_rgb *rgb, int r, int g, int b)
{
	rgb->r = r;
	rgb->g = g;
	rgb->b = b;
}

int	hsv_to_rgb(t_hsv *hsv, t_rgb *rgb)
{
	t_vals			*vals;
	unsigned char	region;
	unsigned char	remainder;

	vals = malloc(sizeof(t_vals));
	region = hsv->h / 43;
	remainder = (hsv->h - (region * 43)) * 6;
	vals->p = (hsv->v * (255 - hsv->s)) >> 8;
	vals->q = (hsv->v * (255 - ((hsv->s * remainder) >> 8))) >> 8;
	vals->t = (hsv->v * (255 - ((hsv->s * (255 - remainder)) >> 8))) >> 8;
	if (region == 0)
		make_t_rgb(rgb, hsv->v, vals->t, vals->p);
	else if (region == 1)
		make_t_rgb(rgb, vals->q, hsv->v, vals->p);
	else if (region == 2)
		make_t_rgb(rgb, vals->p, hsv->v, vals->t);
	else if (region == 3)
		make_t_rgb(rgb, vals->p, vals->q, hsv->v);
	else if (region == 4)
		make_t_rgb(rgb, vals->t, vals->p, hsv->v);
	else
		make_t_rgb(rgb, hsv->v, vals->p, vals->q);
	free(vals);
	return (create_trgb(0, rgb->r, rgb->g, rgb->b));
}

double	mandelbrot(float x0, float y0)
{
	float	x;
	float	y;
	double	iteration;

	x = 0.0;
	y = 0.0;
	iteration = 0;
	while ((x * x + y * y <= 1 << 24) && iteration < MAX_ITER)
	{
		double xtmp = (x * x) - (y * y) + x0;
		y = 2 * y * x + y0;
		x = xtmp;
		iteration += 1;
	}
	if (iteration != MAX_ITER)
		iteration = iteration + 1 - log(log2(x * x + y * y));
	return (iteration);
	// return (floor(iteration));
}

void	plot_mandelbrot(t_img *img)
{
	int		y;
	int		x;
	double	n;
	t_hsv	*hsv;
	t_rgb	*rgb;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			rgb = malloc(sizeof(t_rgb));
			hsv = malloc(sizeof(t_hsv));
			n = mandelbrot(scale_x(x), scale_y(y));
			// returns an amount of iterations which can then be assigned to a color
			hsv->h = ((255 * n) / MAX_ITER);
			hsv->s = 255;
			hsv->v = 255;
			if (n >= MAX_ITER)
				hsv->v = 0;
			// hsv->h = hsv->h % 50;
			img_pix_put(img, x++, y, hsv_to_rgb(hsv, rgb));
			free(rgb);
			free(hsv);
		}
		++y;
	}
}

int	render(t_data *data)
{
	/* if window has been destroyed, we don't want to put pixels */
	if (data->win_ptr == NULL)
		return (1);
	plot_mandelbrot(&data->img);
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
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Fractol");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (MLX_ERROR);
	}
	/* Setup hooks */ 
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);	
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);
	mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);
	mlx_hook(data.win_ptr, 17, 1L<<17, &destroy_exit, &data);
	mlx_loop(data.mlx_ptr);
	/* we will exit the loop if there's no window left, and execute this code */
	mlx_destroy_display(data.mlx_ptr);
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