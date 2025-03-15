/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:16:56 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:21:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	ch;

	if (s == NULL)
		return (NULL);
	ptr = (unsigned char *) s;
	ch = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == ch)
			return (&ptr[i]);
		i++;
	}
	return (NULL);
}
