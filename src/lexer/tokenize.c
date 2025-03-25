/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 18:48:56 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "lexer.h"

static void	add_token(t_list **list, t_token_type type, char *value)
{
	if (list == NULL || value == NULL)
		return ;
	if (ft_strlen(value) > 0)
		ft_lstadd_back(list, ft_lstnew(create_token(type, value)));
	while (*value++ != '\0')
		*value = '\0';
}

t_error	tokenize(t_list **tokens, char *input)
{
	char	*buf;
	size_t	i;
	size_t	j;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (ft_strchr(" \t\n", input[i]))
		{
			j = 0;
			add_token(tokens, TK_WORD, buf);
		}
		else
			buf[j++] = input[i];
		i++;
	}
	add_token(tokens, TK_WORD, buf);
	return (free(buf), ERR_NONE);
}
