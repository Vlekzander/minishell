/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_str_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:55:48 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 16:02:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	remove_str_quotes(char *str)
{
	char	quote;
	size_t	i;
	size_t	len;

	if (str == NULL)
		return ;
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
		{
			len = ft_strlen(str + i + 1);
			quote = handle_quote(str[i], quote);
			ft_memmove(str + i, str + i + 1, len + 1);
		}
		else
			i++;
	}
}
