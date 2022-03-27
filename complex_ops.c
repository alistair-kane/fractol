/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 14:18:57 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 19:39:35 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_cplx	add_comp(t_cplx e1, t_cplx e2)
{
	t_cplx	e3;

	e3.real = e1.real + e2.real;
	e3.imag = e1.imag + e2.imag;
	return (e3);
}

t_cplx	mul_comp(t_cplx e1, t_cplx e2, int burn)
{
	t_cplx	e3;

	if (burn)
	{
		e3.real = fabs(e1.real) * fabs(e2.real) - fabs(e1.imag) * fabs(e2.imag);
		e3.imag = fabs(e1.real) * fabs(e2.imag) + fabs(e2.real) * fabs(e1.imag);
	}
	else
	{
		e3.real = e1.real * e2.real - e1.imag * e2.imag;
		e3.imag = e1.real * e2.imag + e2.real * e1.imag;
	}
	return (e3);
}

double	abs_comp(t_cplx e1)
{
	double	pow1;
	double	pow2;

	pow1 = e1.real * e1.real;
	pow2 = e1.imag * e1.imag;
	return (sqrt(pow1 + pow2));
}
