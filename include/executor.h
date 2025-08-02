/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:32:51 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 17:41:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# include <unistd.h>
# include "data.h"
# include "error.h"
# include "hash.h"

typedef enum e_ret_type
{
	RET_PID,
	RET_VALUE
}	t_ret_type;

typedef enum e_ret_child
{
	RET_SUB_NONE,
	RET_SUB_PREPARE,
	RET_SUB_EXEC
}	t_ret_sub;

typedef struct s_ret
{
	t_ret_type	type;
	t_ret_sub	ret_sub;
	union
	{
		pid_t	pid;
		int		value;
	};
}	t_ret;

t_error	execute_node(t_ast *node, t_hash_table *env);
t_error	execute_command_node(t_ast *node, t_hash_table *env);
t_error	execute_pipeline_node(t_ast *node, t_hash_table *env);
t_error	execute_logic_node(t_ast *node, t_hash_table *env);
t_error	execute_redir_node(t_ast *node, t_hash_table *env);
t_error	execute_command(t_ret *ret, t_list *args, t_list *redirs,
			t_hash_table *env);
t_error	find_executable(char **executable, char *command, t_hash_table *env);
t_error	prepare_command(t_command **command, t_list *args, t_hash_table *env);
int		get_exit_code(t_ret ret, t_error err);

#endif
