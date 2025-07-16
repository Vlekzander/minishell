/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:14 by apierret          #+#    #+#             */
/*   Updated: 2025/07/16 21:43:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_redir	*create_redir(t_redir_type type, char *value, int append)
{
	t_redir	*redir;
	char	*str;

	if (value == NULL)
		return (NULL);
	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	str = ft_strdup(value);
	if (str == NULL)
		return (free_redir(redir), NULL);
	redir->type = type;
	redir->fd = -1;
	if (type == REDIR_IN)
		redir->in = str;
	else if (type == REDIR_OUT)
	{
		redir->out = str;
		redir->append = append;
	}
	else if (type == REDIR_HEREDOC)
		redir->heredoc = str;
	return (redir);
}
