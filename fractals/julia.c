/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 14:22:11 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 22:36:51 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	init_julia_1(t_data *data)
{
	data->re_min = -1.0;
	data->re_max = 1.0;
	data->im_min = -1.2;
	data->im_max = 1.2;
	data->con_real = 0.285;
	data->con_imag = 0.01;
}

void	init_julia_2(t_data *data)
{
	data->re_min = -1.0;
	data->re_max = 1.0;
	data->im_min = -1.2;
	data->im_max = 1.2;
	data->con_real = -0.7269;
	data->con_imag = 0.1889;
}

void	init_julia_3(t_data *data)
{
	data->re_min = -1.0;
	data->re_max = 1.0;
	data->im_min = -1.2;
	data->im_max = 1.2;
	data->con_real = -0.8;
	data->con_imag = 0.156;
}

void	init_julia_4(t_data *data)
{
	data->re_min = -1.0;
	data->re_max = 1.0;
	data->im_min = -1.2;
	data->im_max = 1.2;
	data->con_real = -0.4;
	data->con_imag = 0.6;
}

double	julia(float const_real, float const_imag, t_cplx z0, int max_iter)
{
	float	iteration;
	t_cplx	z;
	t_cplx	c;

	c.real = const_real;
	c.imag = const_imag;
	z = z0;
	iteration = -1.0;
	while (abs_comp(z) <= 2 && ++iteration < max_iter)
		z = add_comp(c, mul_comp(z, z, 0));
	if (iteration == max_iter)
		return (max_iter);
	return (iteration + 1 - log(log2(abs_comp(z))));
}
