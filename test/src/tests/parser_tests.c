/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:28:35 by apierret          #+#    #+#             */
/*   Updated: 2025/04/04 12:16:14 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "parser.h"
#include "tests.h"
#include "test_utils.h"

static t_case ast_cases[] = {
	{
		.name = "single_command",
		.tokens = { "pwd", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "pwd",
				.args = (char*[]){"pwd", NULL},
				.redir = { NULL, NULL, NULL, 0 }
			}
		}
	},
	{
		.name = "one_arg",
		.tokens = { "echo", "hello", NULL },
		.excepted_ast = &(t_ast){
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
		.name = "multiple_args",
		.tokens = { "echo", "hello", "world", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "hello", "world", NULL},
				.redir = { NULL, NULL, NULL, 0 }
			}
		}
	},
	{
		.name = "redir_out",
		.tokens = { "echo", "hello", ">", "out.txt", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL},
				.redir = { NULL, "out.txt", NULL, 0 }
			}
		}
	},
	{
		.name = "redir_append",
		.tokens = { "echo", "world", ">>", "out.txt", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL},
				.redir = { NULL, "out.txt", NULL, 1 }
			}
		}
	},
	{
		.name = "redir_in",
		.tokens = { "cat", "<", "in.txt", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"cat", NULL},
				.redir = { "in.txt", NULL, NULL, 0 }
			}
		}
	},
	{
		.name = "redir_heredoc",
		.tokens = { "cat", "<<", "END", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"cat", NULL},
				.redir = { NULL, NULL, "END", 0 }
			}
		}
	},
	{
		.name = "redir_out_inverted",
		.tokens = { ">", "out.txt", "echo", "hello", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL},
				.redir = { NULL, "out.txt", NULL, 0 }
			}
		}
	},
	{
		.name = "redir_append_inverted",
		.tokens = { ">>", "out.txt", "echo", "world", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL},
				.redir = { NULL, "out.txt", NULL, 1 }
			}
		}
	},
	{
		.name = "redir_in_inverted",
		.tokens = { "<", "in.txt", "cat", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"cat", NULL},
				.redir = { "in.txt", NULL, NULL, 0 }
			}
		}
	},
	{
		.name = "redir_heredoc_inverted",
		.tokens = { "<<", "END", "cat", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command){
				.path = "echo",
				.args = (char*[]){"cat", NULL},
				.redir = { NULL, NULL, "END", 0 }
			}
		}
	},
	{
		.name = "single_pipe",
		.tokens = { "ls", "|", "grep", ".c", NULL },
		.excepted_ast = &(t_ast){
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
		.name = "multiple_pipes",
		.tokens = { "ls", "|", "grep", ".c", "|", "wc", "-l", NULL },
		.excepted_ast = &(t_ast){
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
					.next = &(t_list){
						.content = &(t_command){
							.path = "wc",
							.args = (char*[]){"wc", "-l", NULL},
							.redir = { NULL, NULL, NULL, 0 }
						},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "pipes_and_redir",
		.tokens = { "cat", "<", "in.txt", "|", "grep", "hello", "|", "wc", "-l", ">", "out.txt", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list){
				.content = &(t_command){
					.path = "cat",
					.args = (char*[]){"cat", NULL},
					.redir = { "in.txt", NULL, NULL, 0 }
				},
				.next = &(t_list){
					.content = &(t_command){
						.path = "grep",
						.args = (char*[]){"grep", "hello", NULL},
						.redir = { NULL, NULL, NULL, 0 }
					},
					.next = &(t_list){
						.content = &(t_command){
							.path = "wc",
							.args = (char*[]){"wc", "-l", NULL},
							.redir = { NULL, "out.txt", NULL, 0 }
						},
						.next = NULL
					}
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
	tokens = create_token_list(tc->tokens);
	parse_ast(&tested, tokens);
	equal = ast_equal(tc->excepted_ast, tested);
	free_ast(tested);
	ft_lstclear(&tokens, (void (*)(void *)) free_token);
	if (!equal)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}