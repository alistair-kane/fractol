/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 13:53:07 by alkane            #+#    #+#             */
/*   Updated: 2022/03/28 18:22:46 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	init_mandelbrot(t_data *data)
{
	data->re_min = -2.0;
	data->re_max = 1.0;
	data->im_min = -1.0;
	data->im_max = 1.0;
}

double	mandelbrot(t_cplx cplx, int burn, int max_iter)
{
	float	iteration;
	t_cplx	z;

	z.real = 0.0;
	z.imag = 0.0;
	iteration = -1.0;
	while (abs_comp(z) <= 2 && ++iteration < max_iter)
		z = add_comp(cplx, mul_comp(z, z, burn));
	if (iteration == max_iter)
		return (max_iter);
	return (iteration + 1 - log(log2(abs_comp(z))));
}
