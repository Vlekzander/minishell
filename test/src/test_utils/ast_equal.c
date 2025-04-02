/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:39:20 by apierret          #+#    #+#             */
/*   Updated: 2025/04/02 11:58:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "test_utils.h"

static int	redir_equal(t_redir *a, t_redir *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (strcmp(a->in, b->in) != 0)
		return (0);
	if (strcmp(a->out, b->out) != 0)
		return (0);
	if (strcmp(a->heredoc, b->heredoc) != 0)
		return (0);
	return (a->append == b->append);
}

static int	args_equal(char **a, char **b)
{
	int	i = 0;

	if (!a || !b)
		return (a == b);
	while (a[i] && b[i])
	{
		if (strcmp(a[i], b[i]) != 0)
			return 0;
		i++;
	}
	return (a[i] == b[i]);
}

static int command_equal(t_command *a, t_command *b)
{
	if (!a || !b)
		return (a == b);
	if (strcmp(a->path, b->path) != 0)
		return 0;
	if (!args_equal(a->args, b->args))
		return 0;
	return redir_equal(&a->redir, &b->redir);
}

static int	pipeline_equal(t_list *a, t_list *b)
{
	while (a != NULL && b != NULL)
	{
		if (command_equal((t_command *) a->content, (t_command *) b->content) == 0)
			return (0);
		a = a->next;
		b = b->next;
	}
	return (a == b);
}

int		ast_equal(t_ast *a, t_ast *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (a->type != b->type)
		return (0);
	if (a->type == NODE_COMMAND)
		return (command_equal(a->command, b->command));
	if (a->type == NODE_PIPELINE)
		return (pipeline_equal(a->pipeline, b->pipeline));
	if (a->type == NODE_OR || a->type == NODE_AND)
		return (ast_equal(a->left, b->left) && ast_equal(a->right, b->right));
	if (a->type == NODE_SUBSHELL)
		return (ast_equal(a->child, b->child));
	return (0);
}
