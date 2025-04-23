/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:14 by apierret          #+#    #+#             */
/*   Updated: 2025/04/23 14:14:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_redir	*create_redir(t_token_type type, char *value)
{
	t_redir	*redir;

	if (value == NULL || (type != TK_IN && type != TK_OUT && type != TK_APPEND && type != TK_HEREDOC))
		return (NULL);
	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	if (type == TK_IN)
	{
		redir->type = REDIR_IN;
		redir->in = ft_strdup(value);
	}
	else if (type == TK_OUT || type == TK_APPEND)
	{
		redir->type = REDIR_OUT;
		redir->out = ft_strdup(value);
		redir->append = (type == TK_APPEND);
	}
	else if (type == TK_HEREDOC)
	{
		redir->type = REDIR_HEREDOC;
		redir->heredoc = ft_strdup(value);
	}
	return (redir);
}