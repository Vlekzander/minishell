/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:28:35 by apierret          #+#    #+#             */
/*   Updated: 2025/04/04 15:05:08 by apierret         ###   ########.fr       */
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
				.content = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "ls",
						.args = (char*[]){"ls", NULL},
						.redir = { NULL, NULL, NULL, 0 }
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL},
							.redir = { NULL, NULL, NULL, 0 }
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
						.redir = { NULL, NULL, NULL, 0 }
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL},
							.redir = { NULL, NULL, NULL, 0 }
						}
					},
					.next = &(t_list){
						.content = &(t_ast){
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command){
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL},
								.redir = { NULL, NULL, NULL, 0 }
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
						.redir = { "in.txt", NULL, NULL, 0 }
					}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", "hello", NULL},
							.redir = { NULL, NULL, NULL, 0 }
						}
					},
					.next = &(t_list){
						.content = &(t_ast){
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command){
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL},
								.redir = { NULL, "out.txt", NULL, 0 }
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
					.redir = {NULL, NULL, NULL, 0}
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "./program",
					.args = (char*[]){"./program", NULL},
					.redir = {NULL, NULL, NULL, 0}
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
					.redir = {NULL, NULL, NULL, 0}
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "fail", NULL},
					.redir = {NULL, NULL, NULL, 0}
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
					.redir = {NULL, NULL, NULL, 0}
				}
			},
			.redir = {NULL, NULL, NULL, 0}
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
					.redir = {NULL, "out.txt", NULL, 0}
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "cat",
					.args = (char*[]){"cat", NULL},
					.redir = {"in.txt", NULL, NULL, 0}
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
						.redir = { "in.txt", NULL, NULL, 0 }
					},
					.next = &(t_list){
						.content = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", "world", NULL},
							.redir = { NULL, NULL, NULL, 0 }
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
						.redir = { NULL, NULL, NULL, 0 }
					},
					.next = &(t_list){
						.content = &(t_command){
							.path = "grep",
							.args = (char*[]){"grep", ".h", NULL},
							.redir = { NULL, NULL, NULL, 0 }
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
							.redir = {NULL, NULL, NULL, 0}
						}
					},
					.redir = {NULL, NULL, NULL, 0}
				},
				.next = &(t_list){
					.content = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "wc",
							.args = (char*[]){"wc", "-l", NULL},
							.redir = { NULL, NULL, NULL, 0 }
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
						.redir = { NULL, NULL, NULL, 0 }
					}
				},
				.redir = {NULL, "out.txt", NULL, 0}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "done", NULL},
					.redir = { NULL, NULL, NULL, 0 }
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
					.redir = { NULL, NULL, NULL, 0 }
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
							.redir = { NULL, NULL, NULL, 0 }
						}
					},
					.right = &(t_ast){
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command){
							.path = "echo",
							.args = (char*[]){"echo", "fallback", NULL},
							.redir = { NULL, NULL, NULL, 0 }
						}
					}
				},
				.redir = {NULL, "out.txt", NULL, 0}
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
							.redir = {NULL, NULL, NULL, 0}
						},
					},
					.redir = {NULL, NULL, NULL, 0}
				},
				.redir = {NULL, NULL, NULL, 0}
			},
			.redir = {NULL, NULL, NULL, 0}
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
							.redir = { NULL, NULL, NULL, 0 }
						},
						.next = &(t_list){
							.content = &(t_command){
								.path = "grep",
								.args = (char*[]){"grep", "error", NULL},
								.redir = { NULL, NULL, NULL, 0 }
							},
							.next = NULL
						}
					}
				},
				.redir = {NULL, NULL, NULL, 0}
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
						.redir = { NULL, NULL, NULL, 0 }
					}
				},
				.right = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command){
						.path = "echo",
						.args = (char*[]){"echo", "none", NULL},
						.redir = { NULL, NULL, NULL, 0 }
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