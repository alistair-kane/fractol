/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 19:30:14 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdlib.h>
# include <math.h>
# include <unistd.h>
// # include <X11/X.h>
// # include <X11/keysym.h>
// # include "mlx_linux/mlx.h"
# include "mlx/mlx.h"

# define W_WIDTH 1280
# define W_HEIGHT 720

# define MLX_ERROR 1
# define DEFAULT_ITER 80

# define MLX_SYNC_IMAGE_WRITABLE	1
# define MLX_SYNC_WIN_FLUSH_CMD		2
# define MLX_SYNC_WIN_CMD_COMPLETED	3

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
	int		iterations;
	char	*name;
	double	re_min;
	double	re_max;
	double	im_min;
	double	im_max;
	double	zoom;
	double	con_real;
	double	con_imag;
}	t_data;

typedef struct s_cplx
{
	double	real;
	double	imag;
}	t_cplx;

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


void	init_mandelbrot(t_data *data);
// float rather than double is faster?
double	mandelbrot(t_cplx cplx, int burn, int max_iter);
void	init_julia_1(t_data *data);
void	init_julia_2(t_data *data);
void	init_julia_3(t_data *data);
void	init_julia_4(t_data *data);
double	julia(float const_real, float const_imag, t_cplx z0, int max_iter);
void	init_burningship(t_data *data);

t_cplx	add_comp(t_cplx e1, t_cplx e2);
t_cplx	mul_comp(t_cplx e1, t_cplx e2, int burn);
double	abs_comp(t_cplx e1);

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putstr_fd(const char *s, int fd);
char	*ft_itoa(int n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);

#endif


