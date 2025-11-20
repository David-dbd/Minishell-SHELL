/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:04:23 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Genera y muestra el prompt interactivo.
	Usa el tilde (~) para abreviar la ruta del HOME,
	dando una vista más limpia cuando el usuario está en su directorio personal.
*/

int	display_info_shell_home(char *user, char *host, char *pwd, char **line)
{
	char	*display_shell;

	display_shell = ft_strjoin_multi(9, FUSER, user, "@",
			ft_strcut(host, '.'), FRESET, FPATH, "~", pwd, FEND);
	*line = ic_readline(display_shell);
	free(display_shell);
	return (SUCCESS);
}

/*
	Genera y muestra el prompt interactivo.
	Muestra la ruta de trabajo completa cuando el usuario
	se encuentra fuera de su directorio personal (en la ruta del sistema).
*/

int	display_info_shell_system(char *user, char *host, char *pwd, char **line)
{
	char	*display_shell;

	display_shell = ft_strjoin_multi(8, FUSER, user, "@",
			ft_strcut(host, '.'), FRESET, FPATH, pwd, FEND);
	*line = ic_readline(display_shell);
	free(display_shell);
	return (SUCCESS);
}

/*
	Gestiona el display de shell en modo interactivo.
	Obtiene la información del usuario (HOME, PWD, USER y host) y decide
	si mostrar el prompt con la ruta abreviada (~ para HOME) o la ruta completa,
	Finalmente, almacena la entrada en el historial de 'readline'.
*/

void	terminal_readline(t_shell *data, t_var *vars, char *line)
{
	char	*user;
	char	*home;
	char	*pwd;
	int		*flag;
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
	Controla la lectura de la entrada de comandos por el usuario.
	Si está en un **TTY interactivo**, usa `terminal_readline` con un prompt.
	Si no lo es (pipe/script), lee la entrada directamente con `ic_readline`.
	Verifica el estado de las señales antes de devolver la entrada.
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
	check_signals(data, NULL, NULL);
	return (prompt->input);
}
