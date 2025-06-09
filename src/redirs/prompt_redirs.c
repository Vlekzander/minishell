/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 18:07:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include "data.h"
#include "expand.h"
#include "lexer.h"
#include "redirs.h"

static t_error	prompt_hd(t_strbuilder *sb, int is_last, char *eof, size_t len)
{
	char	*line;
	int		run;

	if (eof == NULL)
		return (ERR_IMPLEMENTATION);
	run = 1;
	while (run)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, eof, len +1) == 0)
			run = 0;
		if (is_last && run)
		{
			if (strbuilder_append(sb, line) != 0)
				return (free(line), free_strbuilder(sb), ERR_ALLOCATION);
			if (strbuilder_append(sb, "\n") != 0)
				return (free(line), free_strbuilder(sb), ERR_ALLOCATION);
		}
		free(line);
	}
	return (ERR_NONE);
}

static t_error	prompt_heredoc(t_redir *redir, int is_last)
{
	t_strbuilder	*sb;
	size_t			eof_len;
	t_error			error;

	if (redir == NULL || redir->type != REDIR_HEREDOC)
		return (ERR_IMPLEMENTATION);
	eof_len = ft_strlen(redir->heredoc);
	sb = NULL;
	if (is_last)
	{
		sb = create_strbuilder(8192);
		if (sb == NULL)
			return (ERR_ALLOCATION);
	}
	error = prompt_hd(sb, is_last, redir->heredoc, eof_len);
	if (error != ERR_NONE)
		return (free_strbuilder(sb), error);
	if (is_last)
		error = process_heredoc(redir, sb);
	return (free_strbuilder(sb), error);
}

static t_error	prompt_hds(t_list *hds, int hd_end)
{
	t_redir	*redir;
	t_list	*node;
	t_error	error;

	if (hds == NULL)
		return (ERR_IMPLEMENTATION);
	node = hds;
	while (node != NULL)
	{
		redir = node->content;
		error = prompt_heredoc(redir, node->next == NULL && hd_end);
		if (error != ERR_NONE)
			return (error);
		node = node->next;
	}
	return (ERR_NONE);
}

static t_error	collect_hds(t_list *redirs, t_list **hds, int *hd_end)
{
	t_redir	*redir;
	t_list	*node;

	*hd_end = 0;
	*hds = NULL;
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == REDIR_HEREDOC)
		{
			node = ft_lstnew(redir);
			if (node == NULL)
				return (ft_lstclear(hds, NULL), ERR_ALLOCATION);
			ft_lstadd_back(hds, node);
			*hd_end = 1;
		}
		else if (redir->type == REDIR_IN)
			*hd_end = 0;
		redirs = redirs->next;
	}
	return (ERR_NONE);
}

t_error	prompt_redirs(t_list *redirs)
{
	t_list	*hds;
	int		hd_end;
	t_error	error;

	error = collect_hds(redirs, &hds, &hd_end);
	if (error != ERR_NONE)
		return (error);
	if (hds != NULL)
	{
		error = prompt_hds(hds, hd_end);
		ft_lstclear(&hds, NULL);
		if (error != ERR_NONE)
			return (error);
	}
	return (ERR_NONE);
}
