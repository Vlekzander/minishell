/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 18:54:31 by apierret         ###   ########.fr       */
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

static void	process_group(char c, char *group)
{
	if (group == NULL)
		return ;
	if (*group == 0)
		*group = c;
	else if (*group == c)
		*group = 0;
}

t_error	tokenize(t_list **tokens, char *input)
{
	char	*buf;
	size_t	i;
	size_t	j;
	char	group;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	group = 0;
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (ft_strchr("'\"", input[i]) && (group == 0 || group == input[i]))
			process_group(input[i], &group);
		else if (group == 0 && ft_strchr(" \t\n", input[i]))
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
