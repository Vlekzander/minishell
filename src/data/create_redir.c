/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:50:56 by apierret          #+#    #+#             */
/*   Updated: 2025/08/05 15:31:56 by apierret         ###   ########.fr       */
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
