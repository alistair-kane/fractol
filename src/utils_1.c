/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 15:10:36 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 19:51:51 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n != 0)
	{
		while (i < n)
		{
			if ((s1[i] != s2[i]) || s1[i] == '\0' || s2[i] == '\0')
				return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
		}
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr_fd(const char *s, int fd)
{
	unsigned int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

static long long	conversion(int n)
{
	long long	converted;

	converted = (long long)n;
	if (n < 0)
		converted *= -1;
	return (converted);
}

char	*ft_itoa(int n)
{
	long long		int_l;
	char			int_array[26];
	char			*int_p;
	unsigned int	i;
	unsigned int	j;

	i = 0;
	int_l = conversion(n);
	while (int_l)
	{
		int_array[i++] = (char)(int_l % 10 + '0');
		int_l /= 10;
	}
	if (n < 0)
		int_array[i++] = '-';
	if (n == 0)
		int_array[i++] = '0';
	int_p = ft_calloc((i + 1), sizeof (char));
	if (!int_p)
		return (0);
	j = 0;
	while (i--)
		int_p[j++] = int_array[i];
	return (int_p);
}
