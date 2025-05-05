/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:28:35 by apierret          #+#    #+#             */
/*   Updated: 2025/05/05 13:27:52 by apierret         ###   ########.fr       */
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
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "pwd",
				.args = (char*[]){"pwd", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "single_command_with_path",
		.tokens = { "/bin/ls", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "/bin/ls",
				.args = (char*[]){"ls", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "one_arg",
		.tokens = { "echo", "hello", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "multiple_args",
		.tokens = { "echo", "hello", "world", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", "world", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "redir_out",
		.tokens = { "echo", "hello", ">", "out.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_append",
		.tokens = { "echo", "world", ">>", "out.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_in",
		.tokens = { "cat", "<", "in.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_heredoc",
		.tokens = { "cat", "<<", "END", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_out_inverted",
		.tokens = { ">", "out.txt", "echo", "hello", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_append_inverted",
		.tokens = { ">>", "out.txt", "echo", "world", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_in_inverted",
		.tokens = { "<", "in.txt", "cat", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_heredoc_inverted",
		.tokens = { "<<", "END", "cat", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
				.next = NULL
			}
		}
	},
	{
		.name = "mult_redir_out",
		.tokens = { "echo", "hello", ">", "out1.txt", ">", "out2.txt", ">", "out3.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out3.txt", .append = 0},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "mult_redir_in",
		.tokens = { "echo", "hello", "<", "in1.txt", "<", "in2.txt", "<", "in3.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .out = "in3.txt", .append = 0},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "mult_redir_out_both_way",
		.tokens = { ">", "out1.txt", ">", "out2.txt", "echo", "hello", ">", "out3.txt", ">", "out4.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_OUT, .out = "out3.txt", .append = 0},
							.next = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_OUT, .out = "out4.txt", .append = 0},
								.next = NULL
							}
						}
					}
				}
		}
	},
	{
		.name = "mult_redir_in_both_way",
		.tokens = { "<", "in1.txt", "<", "in2.txt", "echo", "hello", "<", "in3.txt", "<", "in4.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_IN, .out = "in3.txt", .append = 0},
							.next = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_IN, .out = "in4.txt", .append = 0},
								.next = NULL
							}
						}
					}
				}
		}
	},
	{
		.name = "mult_redirs_both_way",
		.tokens = { "<", "in1.txt", "<", "in2.txt", "echo", "hello", ">", "out1.txt", ">", "out2.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		.name = "single_pipe",
		.tokens = { "ls", "|", "grep", ".c", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list)
			{
				.content = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "ls",
						.args = (char*[]){"ls", NULL}
					},
					.redirs = NULL
				},
				.next = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL}
						},
						.redirs = NULL
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
			.pipeline = &(t_list)
			{
				.content = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "ls",
						.args = (char*[]){"ls", NULL}
					},
					.redirs = NULL
				},
				.next = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "grep",
							.args = (char*[]){"grep", ".c", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL}
							},
							.redirs = NULL
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
		.excepted_ast = &(t_ast)
		{
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list)
			{
				.content = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "cat",
						.args = (char*[]){"cat", NULL}
					},
					.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
						.next = NULL
					}
				},
				.next = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "grep",
							.args = (char*[]){"grep", "hello", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "wc",
								.args = (char*[]){"wc", "-l", NULL}
							},
							.redirs = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
								.next = NULL
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
		.excepted_ast = &(t_ast)
		{
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "make",
					.args = (char*[]){"make", NULL}
				},
				.redirs = NULL
			},
			.right = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "./program",
					.args = (char*[]){"program", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "command_or",
		.tokens = { "mkdir", "folder", "||", "echo", "fail", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_OR,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "mkdir",
					.args = (char*[]){"mkdir", "folder", NULL}
				},
				.redirs = NULL
			},
			.right = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char*[]){"echo", "fail", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "subshell",
		.tokens = { "(", "pwd", ")", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_SUBSHELL,
			.exit_code = 0,
			.child = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "pwd",
					.args = (char*[]){"pwd", NULL}
				},
				.redirs = NULL
			},
			.redirs = NULL
		}
	},
	{
		.name = "and_with_redir",
		.tokens = { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char*[]){"echo", "success", NULL}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
					.next = NULL
				}
			},
			.right = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char*[]){"cat", NULL}
				},
				.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
						.next = NULL
					}
			}
		}
	},
	{
		.name = "or_with_pipe",
		.tokens = { "echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep", ".h", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_OR,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "echo",
							.args = (char*[]){"echo", "hello", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "grep",
								.args = (char*[]){"grep", "world", NULL}
							},
							.redirs = NULL
						},
						.next = NULL
					}
				}
			},
			.right = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "ls",
							.args = (char*[]){"ls", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "grep",
								.args = (char*[]){"grep", ".h", NULL}
							},
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
		.excepted_ast = &(t_ast)
		{
			.type = NODE_PIPELINE,
			.exit_code = 0,
			.pipeline = &(t_list)
			{
				.content = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "ls",
							.args = (char*[]){"ls", "-la", NULL}
						},
						.redirs = NULL
					},
					.redirs = NULL
				},
				.next = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "wc",
							.args = (char*[]){"wc", "-l", NULL}
						},
						.redirs = NULL
					},
					.next = NULL
				}
			},
		}
	},
	{
		.name = "subshell_and_redir",
		.tokens = { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char*[]){"echo", "hello", NULL}
					},
					.redirs = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
					.next = NULL
				}
			},
			.right = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char*[]){"echo", "done", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "nested_logic",
		.tokens = { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "make",
					.args = (char*[]){"make", NULL}
				},
				.redirs = NULL
			},
			.right = &(t_ast){
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_OR,
					.exit_code = 0,
					.left = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "./run",
							.args = (char*[]){"run", NULL}
						},
						.redirs = NULL
					},
					.right = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "echo",
							.args = (char*[]){"echo", "fallback", NULL}
						},
						.redirs = NULL
					}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "nested_logic_two",
		.tokens = { "(", "make", ")", "&&", "./run", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_AND,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "make",
						.args = (char*[]){"make", NULL}
					},
					.redirs = NULL
				}
			},
			.right = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "./run",
					.args = (char*[]){"run", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "deep_nested_group",
		.tokens = { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_SUBSHELL,
			.exit_code = 0,
			.child = &(t_ast)
			{
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "echo",
							.args = (char*[]){"echo", "ok", NULL}
						},
						.redirs = NULL
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
		.excepted_ast = &(t_ast)
		{
			.type = NODE_OR,
			.exit_code = 0,
			.left = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_PIPELINE,
						.exit_code = 0,
						.pipeline = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command = &(t_command)
								{
									.path = "cat",
									.args = (char*[]){"cat", "file", NULL}
								},
								.redirs = NULL
							},
							.next = &(t_list)
							{
								.content = &(t_ast)
								{
									.type = NODE_COMMAND,
									.exit_code = 0,
									.command = &(t_command)
									{
										.path = "grep",
										.args = (char*[]){"grep", "error", NULL}
									},
									.redirs = NULL
								},
								.next = NULL
							}
						}
					},
					.redirs = NULL
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char*[]){"echo", "found", NULL},
					},
					.redirs = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "none", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "redir_out",
		.tokens = { ">", "out.txt", NULL },
		.excepted_ast = &(t_ast)
		{
			.type = NODE_REDIR,
			.exit_code = 0,
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
{
	.name = "redir_in",
	.tokens = { "<", "in.txt", NULL },
	.excepted_ast = &(t_ast)
	{
		.type = NODE_REDIR,
		.exit_code = 0,
		.redirs = &(t_list)
		{
			.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
			.next = NULL
		}
	}
},
	{ NULL }
};

static t_case	error_cases[] = {
	{ .name = "pipe_start", .tokens = {"|"}, .excepted_error = ERR_SYNTAX },
	{ .name = "pipe_end", .tokens = {"cmd", "|"}, .excepted_error = ERR_SYNTAX },
	{ .name = "double_pipe", .tokens = {"cmd", "|", "|", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "logic_start", .tokens = {"&&"}, .excepted_error = ERR_SYNTAX },
	{ .name = "logic_end", .tokens = {"cmd", "||"}, .excepted_error = ERR_SYNTAX },
	{ .name = "logic_double_and", .tokens = {"cmd", "&&", "&&", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "logic_double_or", .tokens = {"cmd", "||", "||", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "logic_mixed_pipe", .tokens = {"cmd", "&&", "|", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "pipe_between_ands", .tokens = {"cmd", "&&", "|", "&&", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "redir_without_target", .tokens = {"cmd", ">"}, .excepted_error = ERR_SYNTAX },
	{ .name = "redir_chain", .tokens = {"cmd", ">", ">", "file"}, .excepted_error = ERR_SYNTAX },
	{ .name = "redir_pipe_combo", .tokens = {"cmd", ">", "|", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "paren_empty", .tokens = {"(", ")"}, .excepted_error = ERR_SYNTAX },
	{ .name = "open_paren_alone", .tokens = {"("}, .excepted_error = ERR_SYNTAX },
	{ .name = "close_paren_alone", .tokens = {")"}, .excepted_error = ERR_SYNTAX },
	{ .name = "unbalanced_parens", .tokens = {"(", "cmd", "|", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "nested_unbalanced_parens", .tokens = {"(", "(", "cmd", ")", "|"}, .excepted_error = ERR_SYNTAX },
	{ .name = "bad_paren_combo", .tokens = {"cmd", ")", "&&", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "mixed_logic_op", .tokens = {"cmd", "&&", "||", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "consecutive_operators", .tokens = {"cmd", "|", "&&", "|", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "pipe_and_or_mix", .tokens = {"cmd", "|", "||", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ .name = "and_or_chain", .tokens = {"cmd", "&&", "||", "&&", "cmd"}, .excepted_error = ERR_SYNTAX },
	{ NULL }
};

MunitResult	parse_ast_basic_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	t_list	*tokens;
	t_ast	*tested;
	t_error	error;
	int		equal;

	(void) data;
	tc = find_case(ast_cases, munit_parameters_get(params, "case"));
	tokens = NULL;
	tested = NULL;
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	tokens = create_token_list(tc->tokens);
	error = parse_ast(&tested, tokens);
	equal = ast_equal(tc->excepted_ast, tested);
	free_ast(tested);
	ft_lstclear(&tokens, (void (*)(void *)) free_token);
	if (!equal || error != ERR_NONE)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}

MunitResult	parse_ast_error_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	t_list	*tokens;
	t_ast	*tested;
	t_error	error;
	int		equal;

	(void) data;
	tc = find_case(error_cases, munit_parameters_get(params, "case"));
	tokens = NULL;
	tested = NULL;
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	tokens = create_token_list(tc->tokens);
	error = parse_ast(&tested, tokens);
	equal = tested == NULL;
	munit_logf(MUNIT_LOG_WARNING, "%d %d %p", equal, error, tested);
	free_ast(tested);
	ft_lstclear(&tokens, (void (*)(void *)) free_token);
	if (!equal || error != tc->excepted_error)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}