/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_wildcar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:41 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 01:08:41 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Elimina todos los caracteres 'trash' de la cadena wildcard.
	Reserva una nueva cadena limpia, copia los caracteres válidos,
	libera la original y devuelve la nueva.
*/

char	*cleanner_wildcar(t_shell *data, char *wildcar, int len, char trash)
{
	int		j;
	int		k;
	char	*clean_wildcar;

	j = 0;
	k = 0;
	if (ft_strchr(wildcar, trash))
	{
		clean_wildcar = ft_calloc(len + 1, sizeof(char));
		if (!clean_wildcar)
		{
			free(wildcar);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		while (wildcar[j])
		{
			if (wildcar[j] != trash)
				clean_wildcar[k++] = wildcar[j];
			j++;
		}
		clean_wildcar[k] = '\0';
		free(wildcar);
		return (clean_wildcar);
	}
	return (wildcar);
}

/*
	Comprueba si un carácter termina una wildcard.
	- Devuelve 1 si es un operador o comilla.
	- Si es ';', lo incorpora al token y termina la wildcard.
*/

static int	isn_wild(int c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')'
		|| c == '$' || c == '\'' || c == '\"' || c == ';')
		return (1);
	return (FALSE);
}

/*
	Funcion para caso edge: *$USER o cauquier otra variable de env.
	De este modo, se pueden separar, expandir y luego unir
*/

static int	if_expansion(t_shell *data, const char *str, int i)
{
	if (i + 1 < ft_strlen(str) && str[i] == '$'
		&& (ft_isalpha(str[i + 1]) || is_symbol(str[i + 1])))
	{
		return (TRUE);
	}
	return (FALSE);
}

/*
	Procesa un patrón de wildcard que empieza con '*'.
	- Recorre hasta espacio o carácter que termina la wildcard.
	- Limpia la cadena de caractees como ';' y '?'.
	- Crea el token WILDCAR
*/

void	is_wildcar(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	int		len;
	int		start;
	int		found_dollar;
	char	*wildcar;
	char	*no_space;

	if (str[*i] == '*')
	{
		start = *i;
		(*i)++;
		while (str[*i] != '\0' && !ft_isspace(str[*i]) && !isn_wild(str[*i]))
			(*i)++;
		if (*i > 0 && str[start] != '\0')
		{
			len = *i - start;
			wildcar = ft_substr(str, start, len);
			if (!wildcar)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			wildcar = cleanner_wildcar(data, wildcar, len, ';');
			wildcar = cleanner_wildcar(data, wildcar, len, '?');
			add_token(data, prompt, wildcar, WILDCARD);
		}
		if (if_expansion(data, str, *i))
			add_token(data, prompt, "", NO_SPACE);
	}
}
