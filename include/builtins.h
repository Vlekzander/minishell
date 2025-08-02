/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:22:25 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 19:14:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "error.h"
# include "hash.h"

typedef t_error	(*t_btin)(int *ret, void *data, t_hash_table *env);

t_btin		get_builtin(char *name);
t_error		builtin_cd(int *ret, void *data, t_hash_table *env);
t_error		builtin_echo(int *ret, void *data, t_hash_table *env);
t_error		builtin_env(int *ret, void *data, t_hash_table *env);
t_error		builtin_exit(int *ret, void *data, t_hash_table *env);
t_error		builtin_export(int *ret, void *data, t_hash_table *env);
t_error		builtin_pwd(int *ret, void *data, t_hash_table *env);
t_error		builtin_unset(int *ret, void *data, t_hash_table *env);

#endif
