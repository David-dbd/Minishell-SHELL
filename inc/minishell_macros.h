/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_macros.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:23:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 17:04:24 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_MACROS_H
# define MINISHELL_MACROS_H

/* ************************************************************************** */
/*                              File Descriptors                             */
/* ************************************************************************** */
# define STDIN						0
# define STDOUT						1
# define STDERR						2

/* ************************************************************************** */
/*                              Boolean Values                                */
/* ************************************************************************** */
# define TRUE						1
# define FALSE						0
# define SUCCESS					0
# define FAILURE					1

/* ************************************************************************** */
/*                              Return Values                                 */
/* ************************************************************************** */
# define ERROR						-1
# define OK							0

/* ************************************************************************** */
/*                               Global Flag                                  */
/* ************************************************************************** */
# define CONTINUE					0
# define CLEAN						1

/* ************************************************************************** */
/*                               Exit Codes                                   */
/* ************************************************************************** */
# define EXIT_SUCCESS				0
# define EXIT_FAILURE				1
# define EXIT_MISUSE				2
# define EXIT_CMD_NOT_FOUND			127
# define EXIT_CMD_NOT_EXECUTABLE	126
# define EXIT_FATAL_SIGNAL			128
# define EXIT_CTRL_C				130
# define EXIT_CTRL_D				131

/* ************************************************************************** */
/*                              File Operations                               */
/* ************************************************************************** */
# define READ_MODE					0
# define WRITE_MODE					1
# define FILE_PERMS					0644
# define DIR_PERMS					0755

/* ************************************************************************** */
/*                              Redirection Types                             */
/* ************************************************************************** */
# define REDIR_INPUT				0		/* < */
# define REDIR_OUTPUT				1		/* > */
# define REDIR_APPEND				2		/* >> */
# define REDIR_HEREDOC				3		/* << */

/* ************************************************************************** */
/*                               Quote States                                 */
/* ************************************************************************** */
# define NO_QUOTE					0
# define SINGLE_QUOTE				1		/* ' */
# define DOUBLE_QUOTE				2		/* " */

/* ************************************************************************** */
/*                               Buffer Sizes                                 */
/* ************************************************************************** */
# define BUFFER_SIZE				1024
# define PATH_MAX_SIZE				4096
# define CMD_MAX_LEN				1024
# define ARG_MAX_COUNT				1024
# define ENV_VAR_MAX				1024
# define PROMPT_MAX					256

/* ************************************************************************** */
/*                              Built-in Commands                             */
/* ************************************************************************** */
# define BUILTIN_ECHO				"echo"
# define BUILTIN_CD					"cd"
# define BUILTIN_PWD				"pwd"
# define BUILTIN_EXPORT				"export"
# define BUILTIN_UNSET				"unset"
# define BUILTIN_ENV				"env"
# define BUILTIN_EXIT				"exit"

/* ************************************************************************** */
/*                              Default Values                                */
/* ************************************************************************** */
# define DEFAULT_PATH				"/usr/bin:/bin:/usr/sbin:/sbin"
# define DEFAULT_PROMPT				"minishell$ "
# define DEFAULT_HOME

/* ************************************************************************** */
/*                               Parser utils                                 */
/* ************************************************************************** */
# define DEFAULT_PATH

/* ************************************************************************** */
/*                              Signal Handling                               */
/* ************************************************************************** */
# define SIG_INTERACTIVE			1
# define SIG_CHILD					2
# define SIG_HEREDOC				3
# define SIG_IGNORE					4

/* ************************************************************************** */
/*                              Error Messages                                */
/* ************************************************************************** */
//General Errors
# define ERR_MALLOC		"minishell: memory allocation failed\n"
# define ERR_FORK		"minishell: fork: Resource temporarily unavailable\n"
# define ERR_PIPE		"minishell: pipe: Too many open files\n"
# define ERR_DUP		"minishell: dup2: Bad file descriptor\n"
# define ERR_SIGNAL		"minishell: signals state failed\n"
# define ERRNO			"Errno: %s\n"

//File Directory Errors
# define ERR_FILE_NOT_FOUND	"minishell: %s: No such file or directory\n"
# define ERR_PERM_DENIED	"minishell: %s: Permission denied\n"
# define ERR_IS_DIR			"minishell: %s: Is a directory\n"
# define ERR_NOT_DIR		"minishell: %s: Not a directory\n"

//Command Errors
# define ERR_CMD_NOT_FOUND	"minishell: %s: command not found\n"
# define ERR_CMD_NOT_EXEC	"minishell: %s: cannot execute binary file\n"
# define ERR_EXEC_FORMAT	"minishell: %s: cannot execute binary file: Exec format error\n"

//Built-in Errors
# define ERR_CD_TOO_MANY	"minishell: cd: too many arguments\n"
# define ERR_CD_NO_HOME		"minishell: cd: HOME not set\n"
# define ERR_CD_OLDPWD		"minishell: cd: OLDPWD not set\n"
# define ERR_EXPORT_INVALID	"minishell: export: `%s': not a valid identifier\n"
# define ERR_UNSET_INVALID	"minishell: unset: `%s': not a valid identifier\n"
# define ERR_EXIT_NUMERIC	"minishell: exit: %s: numeric argument required\n"
# define ERR_EXIT_TOO_MANY	"minishell: exit: too many arguments\n"
# define ERR_PWD_ERROR		"minishell: pwd: error retrieving current directory\n"
# define ERR_ENV_NO_ARGS	"minishell: env: invalid option\n"

//Syntax Errors
# define ERR_SYNTAX			"minishell: syntax error near unexpected token `%s'\n"
# define ERR_SYNTAX_EOF		"minishell: syntax error: unexpected end of file\n"
# define ERR_SYNTAX_NEWLINE	"minishell: syntax error near unexpected token `newline'\n"
# define ERR_UNCLOSED_QUOTE	"minishell: unexpected EOF while looking for matching `%c'\n"
# define ERR_HEREDOC_EOF	"minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n"

//Variable Errors
# define ERR_VAR_NOT_SET	"minishell: %s: parameter null or not set\n"
# define ERR_BAD_SUBST		"minishell: %s: bad substitution\n"

//Redirection Errors
# define ERR_AMBIGUOUS_REDIR	"minishell: %s: ambiguous redirect\n"
# define ERR_NO_INPUT_FILE	"minishell: %s: No such file or directory\n"
# define ERR_REDIR_DENIED	"minishell: %s: Permission denied\n"

/* ************************************************************************** */
/*                              Success Messages                              */
/* ************************************************************************** */
# define MSG_WELCOME		"Welcome to minishell!\n"
# define MSG_GOODBYE		"Goodbye!\n"

#endif
