/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 19:44:59 by alkane            #+#    #+#             */
/*   Updated: 2022/04/01 15:08:27 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

static int	create_trgb(t_rgb rgb)
{
	int	r;
	int	g;
	int	b;

	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	return (0 << 24 | r << 16 | g << 8 | b);
}

static t_rgb	make_t_rgb(int r, int g, int b)
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

void	trip_control(t_data *data)
{
	if (data->hsv_min < 255 && !data->cycle_flag_min)
		data->hsv_min = data->hsv_min + 5;
	else
	{
		data->cycle_flag_min = 1;
		data->hsv_min = data->hsv_min - 5;
		if (data->hsv_min <= 0)
			data->cycle_flag_min = 0;
	}
	if (data->hsv_max < 255 && !data->cycle_flag_max)
		data->hsv_max = data->hsv_max + 5;
	else
	{
		data->cycle_flag_max = 1;
		data->hsv_max = data->hsv_max - 5;
		if (data->hsv_max <= 0)
			data->cycle_flag_max = 0;
	}
}
