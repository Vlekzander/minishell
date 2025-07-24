/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/07/24 14:36:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "data.h"
#include "expand.h"
#include "redirs.h"

extern int	g_signal;

static int	heredoc_event(void)
{
	if (g_signal == SIGINT)
		rl_done = 1;
	return (0);
}

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
		err = expand_env(&expanded, *line, env, NULL);
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

	if (env == NULL || (sb == NULL && is_last) || eof == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	expand = (ft_strchr(eof, '"') == NULL && ft_strchr(eof, '\'') == NULL);
	hd_prepare_eof(eof);
	run = 1;
	while (run)
	{
		line = readline(HEREDOC_PREFIX);
		if (line == NULL || g_signal == SIGINT
			|| ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
			break ;
		if (is_last)
		{
			err = process_line(env, sb, &line, expand);
			if (err.id != ERR_NONE)
				return (free(line), err);
		}
		free(line);
	}
	return (print_heredoc_warn(line, eof), free(line), error(ERR_NONE, NULL));
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
	if (is_last && g_signal != SIGINT)
		err = process_heredoc(redir, sb);
	return (free_strbuilder(sb), err);
}

t_error	prompt_redirs(t_list *redirs, t_hash_table *env)
{
	t_error	err;
	t_redir	*redir;
	t_list	*node;

	if (redirs == NULL)
		return (error(ERR_NONE, NULL));
	err = error(ERR_NONE, NULL);
	g_signal = 0;
	rl_event_hook = heredoc_event;
	node = redirs;
	while (node != NULL && err.id == ERR_NONE && g_signal != SIGINT)
	{
		redir = node->content;
		err = prompt_heredoc(redir, env, node->next == NULL);
		node = node->next;
	}
	if (err.id == ERR_NONE && g_signal == SIGINT)
		err.id = ERR_NO_EXEC;
	return (rl_event_hook = NULL, err);
}
