/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:22:48 by apierret          #+#    #+#             */
/*   Updated: 2025/05/06 22:21:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "data.h"
# include "error.h"
# include "libft.h"

t_error	tokenize(t_list **tokens, char *input);
t_error	globbing(char ***out_files, char **in_files, t_glob *patterns);

#endif
