/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_mask.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:08:40 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 22:57:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "utils.h"

t_error	prepare_mask(char **mask, char *str, int ignore_quote)
{
	int		i;
	char	quote;
	char	*str_mask;

	if (mask == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str_mask = ft_calloc(ft_strlen(str) +1, sizeof(char));
	if (str_mask == NULL)
		return (error(ERR_ALLOCATION, NULL));
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (!ignore_quote && is_quote(str[i])
			&& (quote == 0 || quote == str[i]))
		{
			quote = toggle_quote(str[i], quote);
			str_mask[i] = 'Q';
		}
		else
			str_mask[i] = ' ';
		i++;
	}
	return (*mask = str_mask, error(ERR_NONE, NULL));
}
