/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:15:50 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 13:55:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "test_utils.h"

static t_token_type	get_type(char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (TK_PIPE);
	if (ft_strncmp(str, "<", 2) == 0)
		return (TK_IN);
	if (ft_strncmp(str, ">", 2) == 0)
		return (TK_OUT);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (TK_APPEND);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (TK_HEREDOC);
	if (ft_strncmp(str, "&&", 3) == 0)
		return (TK_AND);
	if (ft_strncmp(str, "||", 3) == 0)
		return (TK_OR);
	if (ft_strncmp(str, "(", 2) == 0)
		return (TK_P_OPEN);
	if (ft_strncmp(str, ")", 2) == 0)
		return (TK_P_CLOSE);
	return (TK_WORD);
}

t_list	*create_token_list(char **strings)
{
	size_t	i;
	t_list	*list = NULL;
	t_token	*token;

	i = 0;
	while (strings[i] != NULL)
	{
		token = create_token(get_type(strings[i]), strings[i]);
		ft_lstadd_back(&list, ft_lstnew(token));
		i++;
	}
	return (list);
}
