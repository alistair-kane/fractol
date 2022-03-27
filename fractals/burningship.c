/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burningship.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 18:38:41 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 19:16:04 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	init_burningship(t_data *data)
{
	data->re_min = -2.5;
	data->re_max = 1.0;
	data->im_min = -1.0;
	data->im_max = 1.0;
}
