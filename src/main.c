/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:49:01 by apierret          #+#    #+#             */
/*   Updated: 2025/07/05 22:48:51 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "env.h"
#include "execution.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include "signals.h"

//TODO Modifier cette fonction pour une bonne gestion d'erreur + de mémoire en cas d'erreur
static void	process_line(char *line, t_hash_table *env, int *run, int *ret)
{
	t_list	*tokens;
	t_ast	*ast;
	t_error	err;

	if (line == NULL)
		return ;
	err = tokenize(&tokens, line);
	if (err.id != ERR_NONE)
		return (print_error(err));
	err = parse_ast(&ast, tokens, env);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&tokens, (void *) free_token), print_error(err));
	err = execute_node(ast, env);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_EXIT)
		{
			*run = 0;
			*ret = ast->exit_code;
		}
		else
			print_error(err);
		ft_lstclear(&tokens, (void *) free_token);
		free_ast(ast);
		return ;
	}
	ft_lstclear(&tokens, (void *) free_token);
	free_ast(ast);
}

int	main(int argc, char **argv, char **envp)
{ (void) argc; (void) argv;
	char			*line;
	int				run;
	int				ret;
	t_hash_table	*env;
	t_error			err;

	err = load_env(&env, envp);
	if (err.id != ERR_NONE)
		return (print_error(err), err.id);
	err = set_var(env, "?", "0");
	if (err.id != ERR_NONE)
		return (print_error(err), err.id);
	setup_signals();
	ret = 0;
	run = 1;
	while (run)
	{
		line = readline("ms > ");
		if (line != NULL)
		{
			if (ft_strlen(line) == 0)
				continue ;
			add_history(line);
			process_line(line, env, &run, &ret);
			free(line);
		}
		else
			process_line("exit", env, &run, &ret);
	}
	return (htable_destroy(env), ret);
}
