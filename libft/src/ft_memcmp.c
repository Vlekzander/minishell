/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:22:38 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:22:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*ptr_s1;
	unsigned char	*ptr_s2;

	if (n == 0)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (INT_MAX);
	ptr_s1 = (unsigned char *) s1;
	ptr_s2 = (unsigned char *) s2;
	i = 0;
	while (i < n -1 && ptr_s1[i] == ptr_s2[i])
		i++;
	return (ptr_s1[i] - ptr_s2[i]);
}
