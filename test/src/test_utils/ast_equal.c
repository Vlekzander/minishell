/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:07:45 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 13:30:19 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	command_equal(t_command *a, t_command *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	if (!str_equal(a->path, b->path))
		return (0);
	if (!str_array_equal(a->args, b->args))
		return (0);
	if (a->redir.append != b->redir.append)
		return (0);
	if (!str_equal(a->redir.in, b->redir.in))
		return (0);
	if (!str_equal(a->redir.out, b->redir.out))
		return (0);
	if (!str_equal(a->redir.heredoc, b->redir.heredoc))
		return (0);
	return (1);
}

static int	pipeline_equal(t_list *a, t_list *b)
{
	if (a == NULL || b == NULL)
		return (a == b);
	while (a != NULL && b != NULL)
	{
		if (!command_equal(a->content, b->content))
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
		return (command_equal(a->command, b->command));
	if (a->type == NODE_PIPELINE)
		return (pipeline_equal(a->pipeline, b->pipeline));
	if (a->type == NODE_AND || a->type == NODE_OR)
		return (ast_equal(a->left, b->left) && ast_equal(a->right, b->right));
	if (a->type == NODE_SUBSHELL)
		return (ast_equal(a->child, b->child));
	return (0);
}
