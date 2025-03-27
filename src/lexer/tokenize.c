/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/03/27 18:52:43 by apierret         ###   ########.fr       */
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

static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static int	is_operator(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '<' && str[1] == '<')
		return (1);
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if (str[0] == '|' && str[1] == '|')
		return (1);
	if (str[0] == '&' && str[1] == '&')
		return (1);
	if (str[0] == '<')
		return (1);
	if (str[0] == '>')
		return (1);
	if (str[0] == '|')
		return (1);
	if (str[0] == '(')
		return (1);
	if (str[0] == ')')
		return (1);
	return (0);
}

static int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static char	handle_quote(char c, char current)
{
	if (current == 0)
		return (c);
	if (current == c)
		return (0);
	return (current);
}

static void process_operator(t_list **tokens, char *input, char *buf, size_t *i)
{
	if (tokens == NULL || input == NULL || buf == NULL || i == NULL)
		return ;
	add_token(tokens, TK_WORD, buf);
	ft_strlcpy(buf, input + *i, 3);
	if (get_token_type(buf) == TK_WORD)
		buf[1] = '\0';
	*i += ft_strlen(buf);
	add_token(tokens, get_token_type(buf), buf);
}

static void process_separator(t_list **tokens, char *buf)
{
	if (tokens == NULL || buf == NULL)
		return;
	add_token(tokens, TK_WORD, buf);
}

t_error	tokenize(t_list **tokens, char *input)
{
	char	*buf;
	char	quote;
	size_t	i;
	size_t	j;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	quote = 0;
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (is_quote(input[i]) && (quote == 0 || quote == input[i]))
			quote = handle_quote(input[i], quote);
		else if (quote == 0 && is_operator(input + i))
		{
			process_operator(tokens, input, buf, &i);
			j = 0;
			continue;
		}
		else if (quote == 0 && is_separator(input[i]))
		{
			process_separator(tokens, buf);
			j = 0;
		}
		else
			buf[j++] = input[i];
		i++;
	}
	add_token(tokens, TK_WORD, buf);
	return (free(buf), ERR_NONE);
}
