/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:28:35 by apierret          #+#    #+#             */
/*   Updated: 2025/04/09 13:43:39 by apierret         ###   ########.fr       */
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
				NULL
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
				NULL
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
				NULL
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
					.next = NULL
				}
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
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
					.next = NULL
				}
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
				.content = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "ls",
						.args = (char*[]){"ls", NULL},
						.redirs = NULL
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL},
							.redirs = NULL
						}
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
				.content = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "ls",
						.args = (char*[]){"ls", NULL},
						.redirs = NULL
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL},
							.redirs = NULL
						}
					},
					.next = &(t_list){
						.content = &(t_ast){
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command){
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL},
								.redirs = NULL
							}
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
				.content = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "cat",
						.args = (char*[]){"cat", NULL},
						.redirs = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
							.next = NULL
						}
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", "hello", NULL},
							.redirs = NULL
						}
					},
					.next = &(t_list){
						.content = &(t_ast){
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command){
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL},
								.redirs = &(t_list)
								{
									.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
									.next = NULL
								}
							}
						},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "command_and",
		.tokens = { "make", "&&", "./program", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "make",
					.args = (char*[]){"make", NULL},
					.redirs = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "./program",
					.args = (char*[]){"./program", NULL},
					.redirs = NULL
				}
			}
		}
	},
	{
		.name = "command_or",
		.tokens = { "./build", "||", "echo", "fail", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_OR,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "make",
					.args = (char*[]){"make", NULL},
					.redirs = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "fail", NULL},
					.redirs = NULL
				}
			}
		}
	},
	{
		.name = "subshell",
		.tokens = { "(", "pwd", ")", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_SUBSHELL,
			.exit_code = 0,
			.child = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "pwd",
					.args = (char*[]){"pwd", NULL},
					.redirs = NULL
				}
			},
			.redirs = NULL
		}
	},
	{
		.name = "and_with_redir",
		.tokens = { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "success", NULL},
					.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
						.next = NULL
					}
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "cat",
					.args = (char*[]){"cat", NULL},
					.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "or_with_pipe",
		.tokens = { "echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep", ".h", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_OR,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list){
					.content = &(t_command){
						.path = "echo",
						.args = (char*[]){"echo", "hello", NULL},
						.redirs = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
							.next = NULL
						}
					},
					.next = &(t_list){
						.content = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", "world", NULL},
							.redirs = NULL
						},
						.next = NULL
					}
				}
			},
			.right = &(t_ast){
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list){
					.content = &(t_command){
						.path = "ls",
						.args = (char*[]){"ls", NULL},
						.redirs = NULL
					},
					.next = &(t_list){
						.content = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".h", NULL},
							.redirs = NULL
						},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "subshell_with_pipe",
		.tokens = { "(", "ls", "-la", ")", "|", "wc", "-l", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list){
				.content = &(t_ast){
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "ls",
							.args = (char*[]){"ls", "-la", NULL},
							.redirs = NULL
						}
					},
					.redirs = NULL
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "wc",
							.args = (char*[]){"wc", "-l", NULL},
							.redirs = NULL
						}
					},
					.next = NULL
				}
			},
		}
	},
	{
		.name = "subshell_and_redir",
		.tokens = { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "echo",
						.args = (char*[]){"echo", "hello", NULL},
						.redirs = NULL
					}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "in.txt", .append = 0},
					.next = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "done", NULL},
					.redirs = NULL
				}
			}
		}
	},
	{
		.name = "nested_logic",
		.tokens = { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "make",
					.args = (char*[]){"make", NULL},
					.redirs = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast){
					.type = NODE_OR,
					.exit_code = 0,
					.left = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "./run",
							.args = (char*[]){"run", NULL},
							.redirs = NULL
						}
					},
					.right = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "echo",
							.args = (char*[]){"echo", "fallback", NULL},
							.redirs = NULL
						}
					}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "deep_nested_group",
		.tokens = { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_SUBSHELL,
			.exit_code = 0,
			.child = &(t_ast){
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast){
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "echo",
							.args = (char*[]){"echo", "ok", NULL},
							.redirs = NULL
						},
					},
					.redirs = NULL
				},
				.redirs = NULL
			},
			.redirs = NULL
		}
	},
	{
		.name = "multi_op_complex",
		.tokens = { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL },
		.excepted_ast = &(t_ast){
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast){
					.type = NODE_PIPELINE,
					.exit_code = 0,
					.pipeline = &(t_list){
						.content = &(t_command){
							.path = "cat",
							.args = (char*[]){"cat", "file", NULL},
							.redirs = NULL
						},
						.next = &(t_list){
							.content = &(t_command){
								.path = "grep",
								.args = (char*[]){"grep", "error", NULL},
								.redirs = NULL
							},
							.next = NULL
						}
					}
				},
				.redirs = NULL
			},
			.right = &(t_ast){
				.type = NODE_OR,
				.exit_code = 0,
				.left = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "echo",
						.args = (char*[]){"echo", "found", NULL},
						.redirs = NULL
					}
				},
				.right = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "echo",
						.args = (char*[]){"echo", "none", NULL},
						.redirs = NULL
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