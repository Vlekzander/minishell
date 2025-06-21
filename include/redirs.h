/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:48 by apierret          #+#    #+#             */
/*   Updated: 2025/06/21 16:06:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRS_H
# define REDIRS_H
# include "data.h"
# include "error.h"
# include "hash.h"

t_error	open_file(int *fd, char *path, t_redir_type type, int mode);
t_error	handle_redirs(t_list *redirs);
t_error	prompt_redirs(t_list *redirs, t_hash_table *env);
t_error	process_heredoc(t_redir *redir, t_strbuilder *sb);

#endif
