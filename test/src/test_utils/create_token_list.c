/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:15:50 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 11:37:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "data.h"
#include "test_utils.h"

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
	if (ft_strncmp(str, "(", 2) == 0)
		return (TK_PARENT_OPEN);
	if (ft_strncmp(str, ")", 2) == 0)
		return (TK_PARENT_CLOSE);
	return (TK_WORD);
}

t_list	*create_token_list(int count, ...)
{
	va_list args;
	t_list	*list = NULL;
	char	*str;
	t_token	*token;

	va_start(args, count);
	for (int i = 0; i < count; i++)
	{
		str = va_arg(args, char *);
		token = create_token(get_type(str), str);
		ft_lstadd_back(&list, ft_lstnew(token));
	}
	va_end(args);
	return (list);
}
