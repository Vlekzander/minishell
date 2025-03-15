/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 11:51:59 by apierret          #+#    #+#             */
/*   Updated: 2024/10/07 11:36:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	int		reverse;
	char	*ptr_dest;
	char	*ptr_src;

	if (dst == src || len == 0)
		return (dst);
	ptr_dest = (char *) dst;
	ptr_src = (char *) src;
	reverse = 0;
	if (ptr_dest > ptr_src && ptr_dest < ptr_src + len)
		reverse = 1;
	i = len * reverse;
	while ((!reverse && i < len) || (reverse && i > 0))
	{
		ptr_dest[i - reverse] = ptr_src[i - reverse];
		if (!reverse)
			i++;
		else
			i--;
	}
	return (dst);
}
