/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:29:21 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:16:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H
# define MASK_NONE ' '
# define MASK_EXPAND 'E'
# define MASK_QUOTE 'Q'
# define MASK_DQUOTE 'D'
# define MASK_SQUOTE 'S'
# include "data.h"
# include "error.h"
# include "hash.h"

typedef struct s_sb_index
{
	t_strbuilder	*sb;
	size_t			index;
}	t_sb_index;

t_error	expand_list(t_list **lst, t_hash_table *env);
t_error	expand_redir_target(char **target, t_hash_table *env);

t_error	variable_expansion(char **str, char **mask, t_hash_table *env);
t_error	word_splitting(t_list **lst, char *str, char *mask, char *ifs);
t_error	pathname_expansion(t_list **lst, char *str, char *mask, char *glob_ign);
t_error	quote_removal(char *str, char *mask);

t_error	create_mask(char **mask, char *str);
t_error	extract_vars(t_list **vars, char *base, char *mask, t_hash_table *env);
t_error	substitute_vars(char **output, char *base, t_list *vars, int is_mask);
t_error	extract_pattern(t_pattern **pattern, char *str, char *mask);
t_error	globbing(t_list **files, t_pattern *pattern, int include);
t_error	pathname_expand_list(t_list **lst, char **mask, t_hash_table *env);

#endif
