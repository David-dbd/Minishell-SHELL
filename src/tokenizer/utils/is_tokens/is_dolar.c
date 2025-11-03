/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   is_dolar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:58:46 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/16 15:52:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Elimina todos los caracteres 'trash' de la cadena 'expansion'.
	Reserva una nueva cadena limpia, copia solo los caracteres válidos,
	libera la original y devuelve la nueva.
	Se usa para quitar '{', '}' de las expansiones de variables.
*/

char	*cleanner_exp(t_shell *data, char *expansion, int len, char trash)
{
	int		j;
	int		k;
	char	*clean_expansion;

	j = 0;
	k = 0;
	if (ft_strchr(expansion, trash))
	{
		clean_expansion = ft_calloc(len + 1, sizeof(char));
		if (!clean_expansion)
		{
			free(expansion);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		while (expansion[j])
		{
			if (expansion[j] != trash)
				clean_expansion[k++] = expansion[j];
			j++;
		}
		clean_expansion[k] = '\0';
		free(expansion);
		return (clean_expansion);
	}
	return (expansion);
}

/*
	Comprueba si el carácter actual termina una expansión o es un símbolo
	especial que debe separarse en otro token.

	- Devuelve 1 si es un operador o símbolo: '|', '<', '>', '&', '(', ')'.
	- Si es un carácter que interrumpe la expansión
	  ('.', comilla, '\', o '$' que inicia otra expansión),
	  marca 'flag = TRUE' para añadir un token NO_SPACE después
	  y retorna 1 para delimitar el token actual.
	- Devuelve 0 si el carácter puede seguir formando parte de la variable.
*/

static int	isn_exp(const char *str, int *i, int str_len, int *flag)
{
	char	c;

	c = str[*i];
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')'
		|| c == ';')
		return (1);
	else if (c == '.' || c == '\'' || c == '\"' || c == '\\')
	{
		*flag = TRUE;
		return (1);
	}
	else if (str[*i - 1] != '$' && str[*i] == '$')
	{
		*flag = TRUE;
		return (1);
	}
	else if (str[*i - 1] == '$' && str[*i] == '$')
	{
		(*i)++;
		if (*i < str_len && !ft_isspace(str[*i + 1]))
			*flag = TRUE;
		return (1);
	}
	return (0);
}

/*
	Crea un token de tipo EXPANSION desde la str.
	Limpia '{' y '}' dentro de la expansión.
	Finalmente añade el token al prompt.
*/

static void	make_expan_token(t_shell *data, const char *str, int start, int *i)
{
	int		len;
	char	*expansion;

	len = *i - start;
	if (len > 0)
	{
		expansion = ft_substr(str, start, len);
		if (!expansion)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		expansion = cleanner_exp(data, expansion, len, '{');
		expansion = cleanner_exp(data, expansion, len, '}');
		if (ft_strcmp(expansion, "$") == 0)
			add_token(data, &data->prompt, expansion, WORD);
		else
			add_token(data, &data->prompt, expansion, EXPANSION);
	}
}

void	handle_tilde_expansion(t_shell *d, t_prompt *p, const char *str, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	(*i)++;
	make_expan_token(d, str, start, i);
	if (str[*i] != '\0' && !ft_isspace(str[*i]))
	{
		start = *i;
		while (str[*i] != '\0' && !ft_isspace(str[*i]))
			(*i)++;
		if (*i > start)
		{
			len = *i - start;
			add_token(d, p, "", NO_SPACE);
			word = ft_substr(str, start, len);
			add_token(d, p, word, WORD);
		}
	}
}

/*
	Procesa un token que comienza con '$':
	- Salta si está seguido de un dígito ($1), que no se expande.
	- Recorre hasta espacio, símbolo especial o fin de cadena.
	- Crea un token EXPANSION con make_expan_token.
	- Añade un token NO_SPACE si el carácter que sigue no forma parte
	  de la variable que debe expandir
*/

void	is_dolar(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	int		flag;
	int		start;

	flag = FALSE;
	if (str[*i] == '$')
	{
		start = *i;
		(*i)++;
		if (ft_isdigit(str[*i]))
		{
			(*i)++;
			return ;
		}
		while (str[*i] != '\0' && !ft_isspace(str[*i])
			&& !isn_exp(str, i, ft_strlen(str), &flag))
			(*i)++;
		make_expan_token(data, str, start, i);
		if (flag == TRUE)
			add_token(data, prompt, "", NO_SPACE);
	}
	if (str[*i] == '~'
		&& (((str[*i + 1] && ft_isspace(str[*i + 1])) || str[*i + 1] == '\0')
			|| (str[*i + 1] && str[*i + 1]== '/')))
		handle_tilde_expansion(data, prompt, str, i);
}
