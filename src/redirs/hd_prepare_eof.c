/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_prepare_eof.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:28:15 by apierret          #+#    #+#             */
/*   Updated: 2025/07/09 17:38:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "redirs.h"
#include "utils.h"

void	hd_prepare_eof(char *str)
{
	int		i;
	size_t	len;
	char	quote;

	if (str == NULL)
		return ;
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
			quote = toggle_quote(str[i], quote);
		if (quote == 0 && str[i] == '$' && is_quote(str[i + 1]))
		{
			len = ft_strlen(str + i + 1);
			ft_memmove(str + i, str + i + 1, len + 1);
		}
		else
			i++;
	}
	remove_str_quotes(str);
}
