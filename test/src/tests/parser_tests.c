/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:28:35 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 13:44:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "parser.h"
#include "tests.h"
#include "test_utils.h"

static t_case ast_cases[] = {
	{
		.name = "simple_echo",
		.tokens = { "echo", "hello", NULL },
		.ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL},
				.redir = { NULL, NULL, NULL, 0 }
			}
		}
	},
	{
		.name = "pipe_ls_grep",
		.tokens = { "ls", "|", "grep", ".c", NULL },
		.ast = &(t_ast){
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list){
				.content = &(t_command){
					.path = "ls",
					.args = (char*[]){"ls", NULL},
					.redir = { NULL, NULL, NULL, 0 }
				},
				.next = &(t_list){
					.content = &(t_command){
						.path = "grep",
						.args = (char*[]){"grep", ".c", NULL},
						.redir = { NULL, NULL, NULL, 0 }
					},
					.next = NULL
				}
			}
		}
	},
	{
		.name = "redir_output",
		.tokens = { "echo", "hi", ">", "out.txt", NULL },
		.ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "hi", NULL},
				.redir = {
					.in = NULL,
					.out = "out.txt",
					.heredoc = NULL,
					.append = 0
				}
			}
		}
	},
	{ NULL }
};

MunitResult	parse_ast_basic_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	t_list	*tokens;
	t_ast	*tested;
	int		equal;

	(void) data;
	tc = find_case(ast_cases, munit_parameters_get(params, "case"));
	tokens = NULL;
	tested = NULL;
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	tokens = create_token_list(tc->expected_tokens);
	parse_ast(&tested, tokens);
	equal = ast_equal(ast_cases->ast, tested);
	free_ast(tested);
	ft_lstclear(&tokens, (void (*)(void *)) free_token);
	if (!equal)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}