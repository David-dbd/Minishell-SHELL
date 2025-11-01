/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_wildcar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:41 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 12:54:22 by davdiaz-         ###   ########.fr       */
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

static int	isn_wild(int c, int *i)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')'
		|| c == '\'' || c == '\"' )
		return (1);
	else if (c == ';')
	{
		(*i)++;
		return (1);
	}
	return (0);
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
	char	*wildcar;

	if (str[*i] == '*')
	{
		start = *i;
		(*i)++;
		while (str[*i] != '\0' && !ft_isspace(str[*i]) && !isn_wild(str[*i], i))
			(*i)++;
		len = *i - start;
		if (len > 0)
		{
			wildcar = ft_substr(str, start, len);
			if (!wildcar)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			wildcar = cleanner_wildcar(data, wildcar, len, ';');
			wildcar = cleanner_wildcar(data, wildcar, len, '?');
			add_token(data, prompt, wildcar, WILDCARD);
		}
	}
}
