/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 23:17:35 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "data.h"
#include "expansion.h"
#include "redirs.h"
#include "signals.h"

extern int	g_signal;

static void	signal_handler(int sig)
{
	g_signal = sig;
	if (sig != SIGINT)
		return ;
	close(STDIN_FILENO);
}

static t_error	process_line(t_hash_table *env, t_strbuilder *sb, char **line,
					int expand)
{
	t_error	err;

	if (env == NULL || sb == NULL || line == NULL || *line == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (expand)
	{
		err = variable_expansion(line, NULL, env);
		if (err.id != ERR_NONE)
			return (err);
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
	t_error	err;

	if (env == NULL || (sb == NULL && is_last) || eof == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	expand = (ft_strchr(eof, '"') == NULL && ft_strchr(eof, '\'') == NULL);
	hd_prepare_eof(eof);
	while (1)
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
	if (is_last && g_signal != SIGINT)
		err = process_heredoc(redir, sb);
	return (free_strbuilder(sb), err);
}

t_error	prompt_redirs(t_list *redirs, t_hash_table *env)
{
	t_error	err;
	t_list	*node;
	int		stdin;

	if (redirs == NULL)
		return (error(ERR_NONE, NULL));
	err = error(ERR_NONE, NULL);
	g_signal = 0;
	signal(SIGINT, signal_handler);
	stdin = dup(STDIN_FILENO);
	if (stdin == -1)
		return (error(ERR_DUP, NULL));
	node = redirs;
	while (node != NULL && err.id == ERR_NONE && g_signal != SIGINT)
	{
		err = prompt_heredoc(node->content, env, node->next == NULL);
		node = node->next;
	}
	if (err.id == ERR_NONE && g_signal == SIGINT)
		err.id = ERR_NO_EXEC;
	setup_signals(0);
	if (dup2(stdin, STDIN_FILENO) == -1)
		err = error(ERR_DUP, NULL);
	return (close(stdin), err);
}
