/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:08:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/06 23:54:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
# include <fcntl.h>
# include "data.h"
# include "error.h"
# include "hash.h"

typedef enum e_ret_type
{
	RET_PID,
	RET_VALUE
}	t_ret_type;

typedef struct s_pipe_fds
{
	int	pipe[2];
	int	input;
}	t_pipe_fds;

typedef struct s_ret
{
	t_ret_type	type;
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
t_error	find_executable(char **executable, char *command, t_hash_table *env);
t_error	prepare_cmd(t_command **command, t_list **args, t_hash_table *env);
t_error	execute_command(t_ret *ret, t_list **cmd_args, t_list *cmd_redirs,
			t_hash_table *env);
int		get_exit_code_pipe(t_ret *rets, int size, t_error err);
int		get_exit_code(t_ret ret, t_error err);
#endif
