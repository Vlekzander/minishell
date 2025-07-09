/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:49:01 by apierret          #+#    #+#             */
/*   Updated: 2025/07/09 16:47:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "env.h"
#include "executor.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include "signals.h"

static t_error	prepare_ast(t_ast **ast, char *line, t_hash_table *env)
{
	t_list	*tokens;
	t_error	err;

	if (ast == NULL || line == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = tokenize(&tokens, line);
	if (err.id != ERR_NONE)
		return (err);
	err = parse_ast(ast, tokens, env);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_SYNTAX)
			set_var(env, "?", "2");
	}
	return (ft_lstclear(&tokens, (void *) free_token), err);
}

static void	process_line(char *line, t_hash_table *env, int *run, int *ret)
{
	t_ast	*ast;
	t_error	err;

	if (line == NULL)
		return ;
	err = prepare_ast(&ast, line, env);
	if (err.id != ERR_NONE)
		return (print_error(err, NULL));
	err = execute_node(ast, env);
	if (err.id != ERR_NONE && err.id != ERR_CMD_EMPTY)
	{
		if (err.id == ERR_EXIT)
			return (*run = 0, *ret = ast->exit_code, free_ast(ast));
		print_error(err, NULL);
	}
	free_ast(ast);
}

static t_error	prepare(t_hash_table **env, int *ret, int *run, char **envp)
{
	char	*str;
	t_error	err;

	err = load_env(env, envp);
	if (err.id != ERR_NONE)
		return (err);
	err = set_var(*env, "?", "0");
	if (err.id != ERR_NONE)
		return (err);
	err = get_var(&str, *env, "OLDPWD");
	if (err.id != ERR_NONE)
		return (err);
	if (ft_strncmp("", str, 1) == 0)
	{
		err = set_var(*env, "OLDPWD", NULL);
		if (err.id != ERR_NONE)
			return (err);
	}
	setup_signals(1);
	*ret = 0;
	*run = 1;
	return (error(ERR_NONE, NULL));
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	int				run;
	int				ret;
	t_hash_table	*env;
	t_error			err;

	err = prepare(&env, &ret, &run, envp);
	if (err.id != ERR_NONE)
		return (print_error(err, NULL), 1);
	while (run)
	{
		line = readline(INPUT_PREFIX);
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
	return ((void) argc, (void) argv, htable_destroy(env), ret);
}
