/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:07:45 by apierret          #+#    #+#             */
/*   Updated: 2025/05/05 13:26:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>
#include <string.h>
#include "test_utils.h"

static int str_equal(char *a, char *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	return (strcmp(a, b) == 0);
}

static int	str_array_equal(char **a, char **b)
{
	int	i;

	if (a == NULL || b == NULL)
		return (a == b);
	i = 0;
	while (a[i] != NULL && b[i] != NULL)
	{
		if (!str_equal(a[i], b[i]))
			return (0);
		i++;
	}
	return (a[i] == b[i]);
}

static int	redir_equal(t_list *a, t_list *b)
{
	t_redir	*redir_a;
	t_redir	*redir_b;
	if (a == NULL || b == NULL)
		return (a == b);
	while (a != NULL && b != NULL)
	{
		redir_a = a->content;
		redir_b = b->content;
		if (redir_a->type != redir_b->type)
			return (0);
		if (redir_a->type == REDIR_IN && !str_equal(redir_a->in, redir_b->in))
			return (0);
		if (redir_a->type == REDIR_OUT && !str_equal(redir_a->out, redir_b->out) && redir_a->append != redir_b->append)
			return (0);
		if (redir_a->type == REDIR_HEREDOC && !str_equal(redir_a->heredoc, redir_b->heredoc))
			return (0);
		a = a->next;
		b = b->next;
	}

	return (a == b);
}

static int	command_equal(t_command *a, t_command *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (!str_equal(a->path, b->path))
		return (0);
	if (!str_array_equal(a->args, b->args))
		return (0);
	return (1);
}

static int	pipeline_equal(t_list *a, t_list *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	while (a != NULL && b != NULL)
	{
		if (!ast_equal(a->content, b->content))
			return (0);
		a = a->next;
		b = b->next;
	}
	return (a == b);
}

int	ast_equal(t_ast *a, t_ast *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (a->type != b->type)
		return (0);
	if (a->type == NODE_COMMAND)
		return (command_equal(a->command, b->command) && redir_equal(a->redirs, b->redirs));
	if (a->type == NODE_PIPELINE)
		return (pipeline_equal(a->pipeline, b->pipeline));
	if (a->type == NODE_AND || a->type == NODE_OR)
		return (ast_equal(a->left, b->left) && ast_equal(a->right, b->right));
	if (a->type == NODE_SUBSHELL)
		return (ast_equal(a->child, b->child) && redir_equal(a->redirs, b->redirs));
	if (a->type == NODE_REDIR)
		return (redir_equal(a->redirs, b->redirs));
	return (0);
}
