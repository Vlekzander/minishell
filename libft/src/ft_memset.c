/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 23:54:26 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:21:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*ptr;

	if (b == NULL || len == 0)
		return (b);
	ptr = (char *) b;
	i = 0;
	while (i < len)
	{
		ptr[i] = (char) c;
		i++;
	}
	return (b);
}
