/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 18:02:08 by alkane            #+#    #+#             */
/*   Updated: 2022/03/27 19:24:21 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	char	*s;
	int		total;

	total = nmemb * size;
	p = malloc(nmemb * size);
	if (!p)
		return (0);
	s = p;
	while (total--)
	{
		*s = '\0';
		s++;
	}
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*dst;
	unsigned int	total_size;
	unsigned int	i;
	unsigned int	j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	total_size = ft_strlen(s1) + ft_strlen(s2) + 1;
	dst = (char *)malloc(sizeof (char) * total_size);
	if (!dst)
		return (NULL);
	while (s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	j = -1;
	while (s2[++j])
		dst[i++] = s2[j];
	dst[i] = '\0';
	free((void *)s2);
	return (dst);
}
