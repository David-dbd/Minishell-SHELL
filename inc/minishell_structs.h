/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:51:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 13:30:34 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H

# include "minishell.h"

typedef struct s_var	t_var;
typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_redir	t_redir;
typedef struct s_node	t_node;
typedef struct s_extras	t_extras;
typedef struct s_cmd	t_cmd;

typedef enum e_type
{
	START,
	WORD,
	GROUP,
	PIPE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	COMMAND,
	SCAPE,
	BUILT_IN,
	SEMICOLON,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	CMD_SUBSTITUTION,
	PAREN_OPEN,
	PAREN_CLOSE,
	WILDCARD,
	EXPANSION,
	ASIGNATION,
	BACKGROUND,
	AND,
	OR,
	EXPAN_VALUE,
	EXPAN_CMD,
	NO_SPACE,
	LOCAL,
	ENV,
	DELETE,
	EXP,
	INDIFERENT,
	PLUS_ASIGNATION,
	TEMP_ASIGNATION,
	FILENAME,
	DELIMITER,
	SUBSHELL,
	SHELL,
	TEMP_PLUS_ASIGNATION,
}	t_type;

struct s_token
{
	int			id;
	int			hash;
	t_type		type;
	char		*value;
	bool		single_quoted;
	bool		double_quoted;
};

typedef struct s_prompt
{
	int			n_parentesis;
	int			n_single_quotes;
	int			n_double_quotes;
	int			n_pipes;
	int			n_cmds;
	int			n_or;
	int			n_and;
	int			n_tokens;
	int			n_alloc_tokens;
	char		*prompt;
	int			*before_tokens_type;
	t_token		*tokens;
}	t_prompt;

struct s_var
{
	char	*key;
	char	*value;
	t_type	type;
	t_var	*next;
	t_var	*prev;
};

typedef struct s_env
{
	char	**envp;
	t_var	*vars;
	int		size;
}	t_env;

typedef struct s_exec
{
	int			original_stdin;
	int			original_stdout;
	t_env		*env;
}	t_exec;

struct	s_redir
{
	t_type		type;
	char		*filename;
	char		*delimiter;
	int			fd_redir;
	int			fd_heredoc;
	bool		signal;
	t_redir		*next;
};

struct s_node
{
	int			id;
	t_type		type;
	t_token		*token;
	int			token_hash;
	char		**args;
	int			*arg_types;
	char		**assig_tmp;
	t_redir		*redir;
	pid_t		pid;
	int			exit_code;
	t_node		*left;
	t_node		*right;
	bool		fake;
	bool		background;
	bool		executed;
	bool		mode;
};

struct s_cmd
{
	char	*value;
	char 	*alias;
	int		active;
	t_cmd	*prev;
	t_cmd	*next;
};

typedef struct s_extras
{
	char		*user_name;
	time_t		session_start;
	t_cmd		*cmd;
	int			n_built_ins;
}	t_extras;

typedef struct s_shell
{
	t_env		env;
	t_prompt	prompt;
	t_exec		exec;
	t_node		*ast_root;
	pid_t		shell_pid;
	t_extras	extra_features;
	int			exit_code;
}	t_shell;

#endif
