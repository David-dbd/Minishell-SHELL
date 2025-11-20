/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 04:19:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:35:59 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 *	Guarda una línea de texto capturada por el heredoc.
 *	Toma la línea de entrada del usuario y la envuelve en un nuevo
 *	nodo de lista. Luego, añade este nodo a la lista `heredoc_lines`
 *	de la redireccion, acumulando asi todo el contenido del documento.
 */

int	save_heredoc_line(t_shell *data, t_redir *redir, char *line)
{
	t_list	*heredoc_line;

	heredoc_line = ft_lstnew(line);
	if (!heredoc_line)
	{
		free(line);
		return (data->error_state = TRUE, ERROR);
	}
	ft_lstadd_back(&redir->heredoc_lines, heredoc_line);
	return (SUCCESS);
}

/*
 *	Bucle principal para capturar la entrada de un heredoc.
 *	Pide lineas al usuario con `ic_readline("> ")` hasta que la entrada
 *	coincida con el `delimiter`. En cada iteracion, se configuran
 *	las señales, se revisa la linea y se guarda. Si se recibe una
 * 	eñal o EOF, el bucle se interrumpe y retorna el estado.
 */

int	loop_heredoc(t_shell *data, t_redir *redir, char *delimiter)
{
	char	*tmp;
	char	*line;
	int		n_line;

	n_line = 0;
	while (42)
	{
		setup_signals_heredoc();
		if (isatty(fileno(stdin)))
			line = ic_readline("> ");
		else // esto es del test se puede borrar y debe borrarse con la
		{
			// return (ft_printf_fd(STDERR, ERR_STDIN), ERROR);
			tmp = get_next_line(fileno(stdin));
			if (!tmp)
				break ;
			line = ft_strtrim(tmp, "\n");
			free(tmp);
		}
		if (!line)
			return (ft_printf_fd(STDERR, ERR_HEREDOC_EOF, n_line, delimiter));
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (check_signals(data, redir, line) == RECIVED_SIGNAL)
			return (RECIVED_SIGNAL);
		if (save_heredoc_line(data, redir, line) == ERROR)
			return (ERROR);
		n_line++;
	}
	return (OK);
}

/*
 *	Funcion de control principal para iniciar la captura del heredoc.
 *	si el dilimitador no estaba entre comillas marca la redireccion
 *	para que las variables se expandan luego. Llama a `loop_heredoc` para
 *	recolectar las lineas y asegura que las señales se restauren
 *	al modo interactivo al finalizar.
 */

int	get_heredoc(t_shell *data, t_redir *redir, char *delimiter, int expansion)
{
	t_list	*heredoc_lines;

	if (expansion == TRUE)
		redir->expand = TRUE;
	if (loop_heredoc(data, redir, delimiter) == ERROR)
		return (setup_signals_interactive(), ERROR);
	setup_signals_interactive();
	return (SUCCESS);
}
