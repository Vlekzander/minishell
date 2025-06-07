/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:04:02 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:37:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# include "data.h"
# include "error.h"

t_error	expand_wildcard(t_list **filtered_files, char *base);
t_error	extract_pattern(t_pattern **pattern, char *str);
t_error	globbing(t_list **out_files, t_list *in_files, t_pattern *pattern);
t_error	expand_env(char **output, char *base, char **env, int ignore_quote);
t_error	extract_var(t_vref **varpos, char *str, int ignore_quote);
t_error	override_vars(char **output, char *base, t_list *vars, char **env);

#endif
