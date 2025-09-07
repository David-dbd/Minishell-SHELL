/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_single_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:59:49 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Elimina todas las comillas simples de una palabra.
	- Crea una nueva cadena sin los caracteres 'quote'.
	- Libera la cadena original y devuelve la limpia.
*/

static char	*cleanner_word(t_shell *data, char *word, int len, char quote)
{
	int		j;
	int		k;
	char	*clean_word;

	j = 0;
	k = 0;
	if (ft_strchr(word, quote))
	{
		clean_word = ft_calloc(len + 1, sizeof(char));
		if (!clean_word)
		{
			free(word);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		while (word[j])
		{
			if (word[j] != quote)
				clean_word[k++] = word[j];
			j++;
		}
		clean_word[k] = '\0';
		free(word);
		return (clean_word);
	}
	return (word);
}

/*
	Convierte un rango de caracteres entre comillas simples en un token WORD.
	- Llama a cleanner_word para quitar las comillas.
	- Si la palabra no está vacía, la añade como token.
	- Aqui no hay expansiones.
	- Registra si es un comando con is_cmd.
*/

void	make_word_s(t_shell *data, t_prompt *p, const char *s, int range[2])
{
	char	*word;
	int		token_id;

	word = ft_substr(s, range[0], range[1] - range[0]);
	if (!word)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	word = cleanner_word(data, word, range[1] - range[0], '\'');
	if (ft_strcmp(word, ""))
	{
		token_id = add_token(data, p, word, WORD);
		is_cmd(data, &data->prompt, &p->tokens[token_id], word);
	}
	else
		free(word);
}

/*
	Comprueba si la comilla actual termina la palabra entre comillas simples.
	- Si siguiente carácter es especial ('$','\','/','\'', letra)
	  marca flag para NO_SPACE para crear token.
	- Maneja escapes '\' y comillas simples consecutivas
	  para incorporarlas al token.
	- Devuelve 1 si la comilla actual cierra la palabra.
*/

int	ft_is_dead_s(const char *s, int *i, char quote, int *flag)
{
	if ((s[*i] == quote && s[*i + 1] != quote))
	{
		if (s[*i + 1] && (ft_isalpha(s[*i + 1]) || s[*i + 1] == '\\'
				|| s[*i + 1] == '$' || s[*i + 1] == '\"' || s[*i + 1] == '/'))
			*flag = TRUE;
		return (1);
	}
	else if (s[*i + 1] && s[*i] == quote && s[*i + 1] == quote)
	{
		if (ft_strchr(s + *i + 2, quote))
			(*i)++;
	}
	return (0);
}

/*
	Procesa una palabra entre comillas simples.
	- Recorre hasta encontrar la comilla de cierre con ft_is_dead_s.
	- Llama a make_word_s para crear token WORD.
	- Retorna NO_SPACE si la palabra termina pegada a un símbolo,
	  TRUE si se procesó normalmente, FALSE si no hay palabra.
*/

int	is_special_word_s(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	int		flag;
	int		len;
	int		start_end[2];

	flag = FALSE;
	if (*i + 1 < ft_strlen(str) && ft_strchr(str + *i, '\''))
	{
		start_end[0] = *i;
		while (str[*i] != '\0' && !ft_is_dead_s(str, i, '\'', &flag))
			(*i)++;
		start_end[1] = *i;
		len = start_end[1] - start_end[0];
		if (len >= 1)
			make_word_s(data, prompt, str, start_end);
		if (flag)
			return (NO_SPACE);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Procesa un token de comillas simples en el input.
	- Añade token de apertura '\''.
	- Extrae y tokeniza la palabra dentro de las comillas.
	- Añade token de cierre '\''.
	- Añade token NO_SPACE si la palabra termina pegada a un símbolo.
*/

void	is_single_quote(t_shell *data, t_prompt *p, const char *str, int *i)
{
	int	status;

	if (str[*i] == '\'')
	{
		add_token(data, p, "\'", SINGLE_QUOTE);
		(*i)++;
		status = is_special_word_s(data, p, str, i);
		if (status == TRUE || status == NO_SPACE)
		{
			add_token(data, p, "\'", SINGLE_QUOTE);
			(*i)++;
			if (status == NO_SPACE)
				add_token(data, p, "", NO_SPACE);
		}
	}
}
