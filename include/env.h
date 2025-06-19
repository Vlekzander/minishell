/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:11:49 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 22:48:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# define ENV_BUCKETS 151
# include "error.h"
# include "hash.h"

t_error	load_env(t_hash_table **env, char **envp);
t_error	get_env(char **str, t_hash_table *env);
t_error	get_var(char **str, t_hash_table *env, char *key);
t_error	set_var(t_hash_table *env, char *key, char *value);
t_error	remove_var(t_hash_table *env, char *key);

#endif
