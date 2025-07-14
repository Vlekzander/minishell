/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_mask.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:57:32 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 17:37:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

t_error	remove_quotes_mask(char *str, char *mask)
{
	int	i;
	int	len;

	if (str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 0;
	while (mask[i] != '\0')
	{
		if (mask[i] == 'Q')
		{
			len = ft_strlen(mask + i + 1);
			ft_memmove(str + i, str + i + 1, len + 1);
			ft_memmove(mask + i, mask + i + 1, len + 1);
		}
		else
			i++;
	}
	return (error(ERR_NONE, NULL));
}
