/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:56:09 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 16:18:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "executor.h"
#include "expand.h"
#include "libft.h"
#include "utils.h"

static t_error	lst_to_arr(char ***array, t_list *lst)
{
	char	**arr;
	size_t	size;
	size_t	i;

	if (lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	size = ft_lstsize(lst);
	arr = ft_calloc(size +1, sizeof(char *));
	if (arr == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (lst != NULL)
	{
		arr[i] = ft_strdup(lst->content);
		if (arr[i] == NULL)
		{
			free_ddarray((void **) arr);
			return (error(ERR_ALLOCATION, NULL));
		}
		i++;
		lst = lst->next;
	}
	return (*array = arr, error(ERR_NONE, NULL));
}

static t_error	prepare_arguments(char ***args, t_list *cmd_args)
{
	char	**arr;
	char	*ptr;
	t_error	err;

	if (args == NULL || cmd_args == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = lst_to_arr(&arr, cmd_args);
	if (err.id != ERR_NONE)
		return (err);
	ptr = ft_strrchr(arr[0], '/');
	if (ptr != NULL)
	{
		if (ptr[1] == '\0')
		{
			free_ddarray((void **) arr);
			return (error(ERR_IS_DIRECTORY, cmd_args->content));
		}
		ft_memmove(arr[0], ptr +1, ft_strlen(ptr));
	}
	return (*args = arr, error(ERR_NONE, NULL));
}

static t_error	prepare_envp(char ***envp, t_hash_table *env)
{
	char	**arr;
	char	*str;
	t_error	err;

	err = get_env(&str, env);
	if (err.id != ERR_NONE)
		return (err);
	arr = ft_split(str, '\n');
	if (arr == NULL)
		return (free(str), err);
	free(str);
	return (*envp = arr, error(ERR_NONE, NULL));
}

t_error	prepare_command(t_command **command, t_list **args, t_hash_table *env)
{
	t_command	*cmd;
	t_error		err;

	err = expand_list(args, env);
	if (err.id != ERR_NONE)
		return (err);
	ft_lstiter(*args, (void *) remove_str_quotes);
	cmd = ft_calloc(1, sizeof(t_command));
	if (cmd == NULL)
		return (error(ERR_ALLOCATION, NULL));
	cmd->builtin = get_builtin((*args)->content);
	if (cmd->builtin == NULL)
	{
		cmd->type = CMD_BINARY;
		err = find_executable(&cmd->executable, (*args)->content, env);
		if (err.id != ERR_NONE)
			return (free_command(cmd), err);
		err = prepare_envp(&cmd->envp, env);
		if (err.id != ERR_NONE)
			return (free_command(cmd), err);
	}
	err = prepare_arguments(&cmd->argv, *args);
	if (err.id != ERR_NONE)
		return (free_command(cmd), err);
	return (cmd->argc = ft_lstsize(*args), *command = cmd, err);
}
