/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:18:14 by apierret          #+#    #+#             */
/*   Updated: 2025/08/03 16:29:36 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "libft.h"
#include "utils.h"

static int	add_env_to_lst(char *key, void *value, void *data)
{
	t_list			**lst;
	size_t			len;
	char			*str;
	t_list			*node;

	if (key == NULL || data == NULL || data + 1 == NULL)
		return (0);
	if (value == NULL || ft_strncmp("?", key, 2) == 0)
		return (1);
	lst = data;
	len = ft_strlen(key) + ft_strlen(value) +1;
	str = ft_calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (0);
	node = ft_lstnew(str);
	if (node == NULL)
		return (free(str), 0);
	ft_strlcat(str, key, len + 1);
	ft_strlcat(str, "=", len + 1);
	ft_strlcat(str, value, len + 1);
	ft_lstadd_back(lst, node);
	return (1);
}

static t_error	prepare_envp(char ***envp, t_hash_table *env)
{
	char	**array;
	t_list	*lst;
	t_error	err;

	if (envp == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	lst = NULL;
	if (!htable_foreach(env, add_env_to_lst, &lst))
		return (ft_lstclear(&lst, free), error(ERR_ALLOCATION, NULL));
	err = lst_array(&array, lst);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&lst, free), err);
	ft_lstclear(&lst, free);
	return (*envp = array, error(ERR_NONE, NULL));
}

static t_error	prepare_arguments(char ***argv, int *argc, t_list *cmd_args)
{
	char	**arr;
	char	*ptr;
	t_error	err;

	if (argv == NULL || cmd_args == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = lst_array(&arr, cmd_args);
	if (err.id != ERR_NONE)
		return (err);
	ptr = ft_strrchr(arr[0], '/');
	if (ptr != NULL && ptr[1] == '\0')
	{
		free_ddarray((void **) arr);
		return (error(ERR_IS_DIRECTORY, cmd_args->content));
	}
	*argv = arr;
	*argc = ft_lstsize(cmd_args);
	return (error(ERR_NONE, NULL));
}

static t_error	prepare_binary(t_command *cmd, t_list *args, t_hash_table *env)
{
	t_error	err;

	if (cmd == NULL || args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	cmd->type = CMD_BINARY;
	err = find_executable(&cmd->executable, args->content, env);
	if (err.id != ERR_NONE)
		return (err);
	err = prepare_envp(&cmd->envp, env);
	if (err.id != ERR_NONE)
		return (err);
	return (error(ERR_NONE, NULL));
}

t_error	prepare_command(t_command **command, t_list *args, t_hash_table *env)
{
	t_command	*cmd;
	t_error		err;

	if (command == NULL || args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	cmd = ft_calloc(1, sizeof(t_command));
	if (cmd == NULL)
		return (error(ERR_ALLOCATION, NULL));
	cmd->builtin = get_builtin(args->content);
	if (cmd->builtin != NULL)
	{
		cmd->stdin = STDIN_FILENO;
		cmd->stdout = STDOUT_FILENO;
	}
	else
	{
		err = prepare_binary(cmd, args, env);
		if (err.id != ERR_NONE)
			return (free_command(cmd), err);
	}
	err = prepare_arguments(&cmd->argv, &cmd->argc, args);
	if (err.id != ERR_NONE)
		return (free_command(cmd), err);
	return (*command = cmd, err);
}
