/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/03/24 19:30:18 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdlib.h>
# include <math.h>
// # include <X11/X.h>
// # include <X11/keysym.h>
// # include "mlx_linux/mlx.h"
# include "mlx/mlx.h"

# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720

# define MLX_ERROR 1
# define MAX_ITER 100

typedef struct s_img
{
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
	int		cur_img;
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	zoom;
}	t_data;

typedef struct s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_rgb;

typedef struct s_hsv
{
	unsigned char	h;
	unsigned char	s;
	unsigned char	v;
}	t_hsv;

typedef struct s_vals
{
	unsigned char	p;
	unsigned char	q;
	unsigned char	t;
}	t_vals;

#endif


