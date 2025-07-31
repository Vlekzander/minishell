/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:07:56 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 12:04:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "utils.h"

static char	get_mask_char(char c)
{
	if (c == '"')
		return ('D');
	if (c == '\'')
		return ('S');
	return (' ');
}

t_error	create_mask(char **mask, char *str)
{
	char	*str_mask;
	int		i;
	char	quote;

	if (mask == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str_mask = ft_calloc(ft_strlen(str) +1, sizeof(char));
	if (str_mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
		{
			quote = toggle_quote(str[i], quote);
			str_mask[i] = 'Q';
		}
		else
			str_mask[i] = get_mask_char(quote);
		i++;
	}
	return (*mask = str_mask, error(ERR_NONE, NULL));
}
