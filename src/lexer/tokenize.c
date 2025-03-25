/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 22:54:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "lexer.h"

static void	add_token(t_list **list, t_token_type type, char *value)
{
	size_t	i;
	if (list == NULL || value == NULL)
		return ;
	if (ft_strlen(value) > 0)
		ft_lstadd_back(list, ft_lstnew(create_token(type, value)));
	i = 0;
	while (value[i] != '\0')
		value[i++]= '\0';
}

static void	handle_group(char c, char *group)
{
	if (group == NULL)
		return ;
	if (*group == 0)
		*group = c;
	else if (*group == c)
		*group = 0;
}

static t_token_type	get_type(char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (TK_PIPE);
	if (ft_strncmp(str, "<", 2) == 0)
		return (TK_REDIR_IN);
	if (ft_strncmp(str, ">", 2) == 0)
		return (TK_REDIR_OUT);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (TK_REDIR_APPEND);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (TK_REDIR_HEREDOC);
	if (ft_strncmp(str, "&&", 3) == 0)
		return (TK_AND);
	if (ft_strncmp(str, "||", 3) == 0)
		return (TK_OR);
	return (TK_WORD);
}

static int	match_operator(char *input)
{
	if (!input)
		return (0);
	if (input[0] == '<' && input[1] == '<')
		return (2);
	if (input[0] == '>' && input[1] == '>')
		return (2);
	if (input[0] == '|' && input[1] == '|')
		return (2);
	if (input[0] == '&' && input[1] == '&')
		return (2);
	if (input[0] == '<')
		return (1);
	if (input[0] == '>')
		return (1);
	if (input[0] == '|')
		return (1);
	return (0);
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
			handle_group(input[i], &group);
		else if (group == 0 && match_operator(input +i) > 0)
		{
			j = 0;
			add_token(tokens, TK_WORD, buf);
			ft_strlcpy(buf, input + i, match_operator(input +i) +1);
			add_token(tokens, get_type(buf), buf);
			i += match_operator(input +i);
			continue;
		}
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
