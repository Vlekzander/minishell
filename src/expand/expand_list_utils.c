/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:57:32 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 23:29:42 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "utils.h"

t_error	prepare_mask(char **mask, char *str)
{
	int		i;
	char	quote;
	char	*str_mask;

	if (mask == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str_mask = ft_strdup(str);
	if (str_mask == NULL)
		return (error(ERR_ALLOCATION, NULL));
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
			str_mask[i] = ' ';
		i++;
	}
	return (*mask = str_mask, error(ERR_NONE, NULL));
}

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
