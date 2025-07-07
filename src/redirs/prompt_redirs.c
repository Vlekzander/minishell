/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 14:26:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include "data.h"
#include "expand.h"
#include "lexer.h"
#include "redirs.h"

static t_error	process_line(t_hash_table *env, t_strbuilder *sb, char **line,
					int expand)
{
	char	*expanded;
	t_error	err;

	if (env == NULL || sb == NULL || line == NULL || *line == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	expanded = NULL;
	if (expand)
	{
		err = expand_env(&expanded, *line, env, 1);
		if (err.id != ERR_NONE)
			return (err);
		if (expanded != *line)
		{
			free(*line);
			*line = expanded;
		}
	}
	if (!strbuilder_append(sb, *line))
		return (error(ERR_ALLOCATION, NULL));
	if (!strbuilder_append(sb, "\n"))
		return (error(ERR_ALLOCATION, NULL));
	return (error(ERR_NONE, NULL));
}

static t_error	prompt_hd(t_hash_table *env, t_strbuilder *sb, int is_last,
					char *eof)
{
	char	*line;
	int		expand;
	int		run;
	t_error	err;

	if (env == NULL || sb == NULL || eof == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	expand = (ft_strchr(eof, '"') == NULL && ft_strchr(eof, '\'') == NULL);
	remove_str_quotes(eof);
	run = 1;
	while (run)
	{
		line = readline(HEREDOC_PREFIX);
		if (line == NULL || ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
			break ;
		if (is_last)
		{
			err = process_line(env, sb, &line, expand);
			if (err.id != ERR_NONE)
				return (free(line), err);
		}
		free(line);
	}
	return (free(line), error(ERR_NONE, NULL));
}

static t_error	prompt_heredoc(t_redir *redir, t_hash_table *env, int is_last)
{
	t_strbuilder	*sb;
	t_error			err;

	if (redir == NULL || redir->type != REDIR_HEREDOC)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = NULL;
	if (is_last)
	{
		sb = create_strbuilder(8192);
		if (sb == NULL)
			return (error(ERR_ALLOCATION, NULL));
	}
	err = prompt_hd(env, sb, is_last, redir->heredoc);
	if (err.id != ERR_NONE)
		return (free_strbuilder(sb), err);
	if (is_last)
		err = process_heredoc(redir, sb);
	return (free_strbuilder(sb), err);
}

static t_error	collect_hds(t_list *redirs, t_list **hds, int *hd_end)
{
	t_redir	*redir;
	t_list	*node;

	if (hds == NULL || hd_end == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*hd_end = 0;
	*hds = NULL;
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == REDIR_HEREDOC)
		{
			node = ft_lstnew(redir);
			if (node == NULL)
				return (ft_lstclear(hds, NULL), error(ERR_ALLOCATION, NULL));
			ft_lstadd_back(hds, node);
			*hd_end = 1;
		}
		else if (redir->type == REDIR_IN)
			*hd_end = 0;
		redirs = redirs->next;
	}
	return (error(ERR_NONE, NULL));
}

t_error	prompt_redirs(t_list *redirs, t_hash_table *env)
{
	t_list	*hds;
	int		hd_end;
	t_error	err;
	t_redir	*redir;
	t_list	*node;

	err = collect_hds(redirs, &hds, &hd_end);
	if (err.id != ERR_NONE)
		return (err);
	if (hds != NULL)
	{
		node = hds;
		while (node != NULL)
		{
			redir = node->content;
			err = prompt_heredoc(redir, env, node->next == NULL && hd_end);
			if (err.id != ERR_NONE)
				return (ft_lstclear(&hds, NULL), err);
			node = node->next;
		}
		ft_lstclear(&hds, NULL);
	}
	return (error(ERR_NONE, NULL));
}
