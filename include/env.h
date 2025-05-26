/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:11:49 by apierret          #+#    #+#             */
/*   Updated: 2025/05/26 16:25:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "error.h"

t_error	load_env(char ***env, char **envp);
t_error	get_env(char **str, char **env);
t_error	get_var(char **str, char **env, char *key);
t_error	set_var(char ***env, char *key, char *value);
t_error	remove_var(char ***env, char *key);

#endif
