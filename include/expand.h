/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:04:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 17:27:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# include "data.h"
# include "error.h"
# include "hash.h"

t_error	expand_wildcard(char **output, char *base, char *mask);
t_error	extract_pattern(t_pattern **pattern, char *str, char *mask);
t_error	globbing(t_list **out_files, t_list *in_files, t_pattern *pattern);
t_error	extract_vars(t_list **vars, char *str, int i_quote, t_hash_table *env);
t_error	substitute_vars(char **output, char *base, t_list *vars, int is_mask);
t_error	expand_list(t_list **lst, t_hash_table *env);
t_error	expand_env(char **output, char *base, t_hash_table *env, int ign_quote);

#endif
