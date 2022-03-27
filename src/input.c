/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 19:47:10 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 22:25:32 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

static double	interp(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

static void	arrow_keys(t_data *data, int code)
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
	if (code == 96 && data->iterations > 0)
		data->iterations = data->iterations - 20;
	if (code == 97)
		data->iterations = data->iterations + 20;
	if (code >= 123 && code <= 126)
		arrow_keys(data, code);
	return (0);
}

static void	zoom(t_data *data, int mouse_x, int mouse_y, int zoom_direction)
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
