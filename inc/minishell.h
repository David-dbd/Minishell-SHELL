/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:07:02 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 17:20:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
/* ************************************************************************** */
/*                               Includes                                     */
/* ************************************************************************** */
# include "../lib/libft_plus.h"
# include "minishell_structs.h"
# include "minishell_macros.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <strings.h>
# include <termios.h>
# include <dirent.h>
# include <signal.h>
# include <errno.h>
# include <term.h>

/* ************************************************************************** */
/*                                 Globales                                   */
/* ************************************************************************** */
extern volatile sig_atomic_t	g_signal_event;

/* ************************************************************************** */
/*                                Minishell                                   */
/* ************************************************************************** */
int		steps_manager(t_shell *data, char *input);
char	*recieve_input(char **input, t_shell *data);

/* ************************************************************************** */
/*                             Initialization                                 */
/* ************************************************************************** */
void	init_data(t_shell *data, char **input);
void	allocate_tokens(t_shell *data, t_prompt *prompt, char *input);

/* ************************************************************************** */
/*                               Tokenizer                                    */
/* ************************************************************************** */
int		tokenizer(t_shell *data, t_prompt *prompt, char *input);
void	get_tokens(t_shell *data, t_token *tokens, char *input);
int		valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens);
int		add_token(t_token *tokens, char *value, int type);

/* ************************************************************************** */
/*                                  AST                                       */
/* ************************************************************************** */
void	ast_built(t_shell *data, t_token *tokens);
/* ************************************************************************** */
/*                                Executor                                    */
/* ************************************************************************** */
void	execute_recursive(t_shell *data, t_node *ast_root, t_exec *executor);

/* ************************************************************************** */
/*                               Expansion                                    */
/* ************************************************************************** */
void	expansion(t_shell *data, t_token *tokens, t_env *env);

/* ************************************************************************** */
/*                                Signals                                     */
/* ************************************************************************** */
int		init_signals(void);
void	signal_handler(int sig, siginfo_t *info, void *context);

/* ************************************************************************** */
/*                                 Exits                                      */
/* ************************************************************************** */
int		exit_error(t_shell *data, char *error, int exit_code);
int		exit_succes(t_shell *data, char *msg, int exit_code);
int		syntax_error(t_shell *data, char *error, int exit_code);

/* ************************************************************************** */
/*                                 Clean                                      */
/* ************************************************************************** */
void	clean_all(t_shell *data);
void	clean_prompt(t_prompt *prompt);
void	clean_tokens(t_prompt *prompt);

/* ************************************************************************** */
/*                                 utils                                      */
/* ************************************************************************** */
// GET TOKENS
void	is_cmd(t_shell *d, t_prompt *p, t_token *t, char *s);
void	is_word(t_shell *data, t_token *tokens, const char *str, int *i);
void	is_dolar(t_shell *data, t_token *tokens, const char *str, int *i);
void	is_quote(t_token *tokens, const char *str, int *i);
void	is_pipe(t_token *tokens, const char *str, int *i);
void	is_parenten(t_token *tokens, const char *str, int *i);
void	is_semicolon(t_token *tokensmm, const char *str, int *i);
void	is_wildcar(t_token *tokens, const char *str, int *i);
void	is_scape(t_token *tokens, const char *str, int *i);
void	is_or(t_token *tokens, const char *str, int *i);
void	is_and(t_token *tokens, const char *str, int *i);
void	is_redir(t_token *tokens, const char *str, int *i);
void	is_heredoc(t_token *tokens, const char *str, int *i);
void	is_not_token(const char *str, int *i);
void	is_hash(const char *str, int *i);
void	reset_tokens(void);

//VALID TOKENS
int		check_open_parent(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_close_parent(t_shell *d, t_prompt *p, t_token *t, int i);
void	check_redir_input(t_shell *d, t_prompt *p, t_token *t, int i);
void	check_redir_output(t_shell *d, t_prompt *p, t_token *t, int i);
void	check_semicolon(t_shell *d, t_prompt *p, t_token *t, int i);
void	check_background(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		check_or_and(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		valid_pair_operands(t_shell *data, t_prompt *prompt);
void	logic_trans_args_cmd(t_shell *data, t_token *tokens);

#endif
