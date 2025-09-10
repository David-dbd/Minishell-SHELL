/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:31:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 22:33:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
/* ************************************************************************** */
/*                               Includes                                     */
/* ************************************************************************** */
# include "../lib/libft_plus/libft_plus.h"
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
# include <time.h>

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
void	init_data(t_shell *data, char **input, char **envp);
void	allocate_tokens(t_shell *data, t_prompt *prompt, char *input);
void	init_enviroment(t_shell *data, char **envp);
void	add_var(t_shell *data, char *key, char *value, int type);

/* ************************************************************************** */
/*                               Tokenizer                                    */
/* ************************************************************************** */
int		tokenizer(t_shell *data, t_prompt *prompt, char *input);
void	parse_tokens(t_shell *data, t_prompt *prompt, char *input);
int		valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens);
int		add_token(t_shell *data, t_prompt *prompt, char *value, int type);

/* ************************************************************************** */
/*                                  AST                                       */
/* ************************************************************************** */
void	ast_built(t_shell *data, t_token *tokens);
/* ************************************************************************** */
/*                                Executor                                    */
/* ************************************************************************** */
void	execute_recursive(t_shell *data, t_node *ast_root, t_exec *executor);
void	which_builtin(t_shell *data, t_token *tokens, t_token *token);

/* ************************************************************************** */
/*                               Expansion                                    */
/* ************************************************************************** */
int		expansion(t_shell *data, t_token *token, t_env *env, int phase);
int		send_tokens_for_expansion(t_shell *data, t_token *tokens, int phase);

/* ************************************************************************** */
/*                                buil_in                                     */
/* ************************************************************************** */
void	my_env(t_var *vars);
void	my_echo(t_token *tokens);
void	my_pwd(t_shell *data);
int		my_export(t_shell *data, t_token *tokens, t_env *env);

/* ************************************************************************** */
/*                                Signals                                     */
/* ************************************************************************** */
int		init_signals(void);
void	signal_handler(int sig, siginfo_t *info, void *context);

/* ************************************************************************** */
/*                                 Exits                                      */
/* ************************************************************************** */
int		exit_error(t_shell *data, const char *error, int exit_code, ...);
int		exit_succes(t_shell *data, char *msg, int exit_code);
int		syntax_error(t_shell *data, const char *error, int exit_code, ...);

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
void	is_word(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_dolar(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_single_quote(t_shell *d, t_prompt *prompt, const char *s, int *i);
void	is_double_quote(t_shell *d, t_prompt *prompt, const char *s, int *i);
void	is_wildcar(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_scape(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_redir(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_heredoc(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_semicolon(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_cmdsubs(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_pipe(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_or(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_and(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_parenten(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_not_token(const char *str, int *i);
void	is_hash(const char *str, int *i);
int		is_cmd_type(int type);
int		is_quote_type(int type);
int		is_redir_type(int type);
int		is_delimiter_type(int type);
int		is_alloc_type(int type);
int		is_between_quotes_type(int type);
int		is_simplify_type(int type);

//VALID TOKENS
int		check_open_parent(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_close_parent(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_redir_input(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_redir_output(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_semicolon(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_background(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		check_or_and(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		check_cmd_syntax(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_parent_balance(t_shell *data, t_prompt *prompt, t_token *tokens);
int		check_double_balance(t_shell *d, t_prompt *p, t_token *t);
int		check_single_balance(t_shell *d, t_prompt *p, t_token *t);

//SIMPLIFY TOKENS
void	simplify_tokens(t_shell *data, t_prompt *prompt, t_token *tokens);
void	reorganize_tokens(t_prompt *p, t_token *tokens, int *range, char *res);
int		find_range_start(t_token *tokens, int no_space_position);
int		find_range_end(t_token *tokens, int no_space_position);
void	remove_quotes_tokens(t_prompt *prompt, t_token *tokens);

//TRANSFORM TOKENS
void	transform_word_to_cmd(t_shell *data, t_token *tokens);
void	transform_word_to_asignation(t_shell *data, t_token *tokens);

//EXPANSION
int		calculate_total_length(t_shell *data, char *str, char *key_to_find);
int		copy_key(char *buffer, char **key_to_find);
int		copy_value(t_shell *d, char **t_val, char *key_value, char *key_to_f);
int		extract_key(t_shell *d, t_token *t, char **key_to_f, int phase);
int		find_key_in_lst(t_shell *d, t_token *t, char *key_to_f);

//ASIGNATION
int		asignation(t_shell *data, t_token *token, int type);
int		is_it_asig(t_shell *data, t_token *token, t_env *env, int type);
int		check_asignation_syntax(t_token *token);
int		check_externs_syntax(t_shell *data, t_token *token);

//ENV
void	path_null_no_env(t_shell *data, char **path);

//UTILS
char	*cleanner_slash(t_shell *data, char *word, int len, char slash);
void	void_tokens_at_the_end(t_token *tokens, int n_alloc, int n_tokens);

/* ************************************************************************** */
/*                               extras - time                                */
/* ************************************************************************** */
void	print_session_start(time_t start);
void	print_session_end(time_t start);
void	print_time_of_day(time_t start);

#endif
