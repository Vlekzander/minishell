/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:22:48 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 22:32:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "data.h"
# include "error.h"
# include "libft.h"

t_error	tokenize(t_list **tokens, char **env, char *input);
t_error	globbing(t_list **out_files, t_list *in_files, t_pattern *pattern);
t_error	expand(t_list **tk_list, char **env);

#endif
