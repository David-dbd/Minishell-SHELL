/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/06 22:39:33 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	display_info_shell_home(char *user, char *host, char *pwd, char **line)
{
	char	*display_shell;

	display_shell = ft_strjoin_multi(9, FUSER, user, "@",
		ft_strcut(host, '.'), FRESET, FPATH, "~", pwd, FEND);
	*line = ic_readline(display_shell);
	free(display_shell);
}

void	display_info_shell_system(char *user, char *host, char *pwd, char **line)
{
	char	*display_shell;

	display_shell = ft_strjoin_multi(8, FUSER, user, "@",
		ft_strcut(host, '.'), FRESET, FPATH, pwd, FEND);
	*line = ic_readline(display_shell);
	free(display_shell);
}

void	terminal_readline(t_shell *data, t_var *vars, char *line)
{
	char	*user;
	char	*home;
	char	*pwd;
	char	host[HOST_NAME_MAX + 1];

	home = get_var_value(vars, "HOME");
	pwd = get_var_value(vars, "PWD");
	user = get_var_value(vars, "USER");
	gethostname(host, HOST_NAME_MAX + 1);
	if (home && pwd && user)
	{
		if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
			display_info_shell_home(user, host, pwd + ft_strlen(home), &line);
		else
			display_info_shell_system(user, host, pwd, &line);
	}
	else
		line = ic_readline(DEFAULT_PROMPT);
	data->prompt.input = read_until_balanced(data, line);
	if (data->prompt.input)
		ic_history_add(data->prompt.input);
}

/*
 * Lee entrada del usuario con readline personalizado: almacena
 * en historial, verifica señales y retorna NULL con Ctrl+D
 * terminando el programa al finalizar el while y saliendo del programa
 */

char	*receive_input(t_shell *data, t_prompt *prompt)
{
	char	*line;

	line = NULL;
	if (isatty(fileno(stdin)))
		terminal_readline(data, data->env.vars, line);
	else
	{
		// ft_printf_fd(STDERR, ERR_STDIN); // se debe entregar asi para la eval
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		prompt->input = ft_strtrim(line, "\n");
		free(line);
	}
	check_signals(data, NULL, NULL, NULL);
	return (prompt->input);
}
