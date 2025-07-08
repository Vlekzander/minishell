/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:22:25 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 23:28:31 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "error.h"
# include "hash.h"

typedef struct s_btin_data
{
	int		forked;
	int		stdin;
	int		stdout;
	int		argc;
	char	**argv;
}	t_btin_data;

typedef t_error	(*t_btin)(int *ret, t_btin_data data, t_hash_table *env);

t_btin		get_builtin(char *name);
t_error		builtin_cd(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_echo(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_env(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_exit(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_export(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_pwd(int *ret, t_btin_data data, t_hash_table *env);
t_error		builtin_unset(int *ret, t_btin_data data, t_hash_table *env);

#endif
