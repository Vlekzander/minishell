/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:04:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/15 00:30:55 by apierret         ###   ########.fr       */
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
t_error	extract_vars(t_list **vars, char *base, char *mask, t_hash_table *env);
t_error	substitute_vars(char **output, char *base, t_list *vars, int is_mask);
t_error	expand_list(t_list **lst, t_hash_table *env);
t_error	expand_env(char **output, char *base, t_hash_table *env, char *mask);
t_error	expand_redir_target(t_redir *redir, t_hash_table *env);
t_error	split_list_mask(t_list **lst, char *str, char *mask, char quote);
t_error	prepare_mask(char **mask, char *str, int ignore_quote);
t_error	remove_quotes_mask(char *str, char *mask);

#endif
