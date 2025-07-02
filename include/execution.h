/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:08:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/02 01:05:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
# include "data.h"
# include "error.h"
# include "hash.h"

t_error	execute_node(t_ast *node, t_hash_table *env);
t_error	execute_cmd_node(t_ast *node, t_hash_table *env);
t_error	execute_pipeline_node(t_ast *node, t_hash_table *env);
t_error	execute_logic_node(t_ast *node, t_hash_table *env);
t_error	execute_redir_node(t_ast *node, t_hash_table *env);
t_error	find_executable(char **executable, char *command, t_hash_table *env);

#endif
