/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_str_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 16:17:14 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:22:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
