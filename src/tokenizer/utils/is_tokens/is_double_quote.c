/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_double_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:17 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Elimina todas las comillas especificadas ('quote') de la palabra.
	Teniendo en cuenta los casos especiales de slashes.
	Reserva una nueva cadena limpia, copia solo los caracteres válidos,
	libera la original y devuelve la nueva.
*/

static char	*cleanner_word(t_shell *data, char *word, int len, char quote)
{
	char	*clean_word;

	if (ft_strchr(word, quote))
	{
		clean_word = ft_calloc(len + 1, sizeof(char));
		if (!clean_word)
		{
			free(word);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		clean_quote_until_slash_d(word, clean_word, quote);
		free(word);
		return (clean_word);
	}
	return (word);
}

/*
	Crea un token WORD o EXPANSION dentro de comillas dobles.
	- Limpia las comillas.
	- Si contiene '$' que puede expandirse, crea token EXPANSION.
	- Registra como comando si corresponde.
*/

void	make_word_d(t_shell *data, t_prompt *promp, const char *s, int range[2])
{
	char	*ptr;
	char	*word;
	int		token_id;
	int		flag;

	flag = FALSE;
	word = ft_substr(s, range[0], range[1] - range[0]);
	if (!word)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	word = cleanner_word(data, word, range[1] - range[0], '\"');
	word = cleanner_slash_quotes_d(data, word, range[1] - range[0], &flag);
	ptr = ft_strchr(word, '$');
	if (ptr)
	{
		if (flag == TRUE)
			token_id = add_token(data, promp, word, WORD);
		else
			token_id = add_token(data, promp, word, EXPANSION);
	}
	else
		token_id = add_token(data, promp, word, WORD);
	is_cmd(data, &data->prompt, &promp->tokens[token_id], word);
}

/*
	Comprueba si la comilla actual termina la palabra.
	- Si siguiente carácter es especial ('$','\','/','\'', letra)
	  marca flag para NO_SPACE para crear token.
	- Maneja escapes '\' y comillas dobles consecutivas
	  para incorporarlas al token.
	- Devuelve 1 si la comilla actual cierra la palabra, 0 si no.
*/

int	ft_is_dead_d(const char *s, int *i, char quote, int *flag)
{
	if ((s[*i] == quote && s[*i + 1] == quote))
	{
		if (s[*i + 2])
			*flag = TRUE;
		return (1);
	}
	else if ((s[*i] == quote && s[*i + 1] != quote))
	{
		if (s[*i + 1] && (ft_isalpha(s[*i + 1]) || s[*i + 1] == '\\'
				|| s[*i + 1] == '$' || s[*i + 1] == '\'' || s[*i + 1] == '/'
				|| s[*i + 1] == '?'))
			*flag = TRUE;
		return (1);
	}
	else if (s[*i + 1] && s[*i] == '\\' && s[*i + 1] != '\0')
	{
		(*i)++;
		return (0);
	}
	else if (s[*i + 1] && s[*i] == quote && s[*i + 1] == quote)
	{
		if (ft_strchr(s + *i + 2, quote))
			(*i)++;
	}
	return (0);
}

/*
	Procesa una palabra dentro de comillas dobles.
	- Recorre hasta la comilla de cierre usando ft_is_dead_d.
	- Llama a make_word_d para crear token.
	- Retorna NO_SPACE si la palabra termina pegada a un carácter especial.
	- Retorna TRUE si se procesó normalmente.
*/

int	is_special_word_d(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	int		flag;
	int		len;
	int		start_end[2];

	flag = FALSE;
	if (*i + 1 < ft_strlen(str) && ft_strchr(str + *i, '\"'))
	{
		start_end[0] = *i;
		while (str[*i] != '\0' && !ft_is_dead_d(str, i, '\"', &flag))
			(*i)++;
		start_end[1] = *i;
		len = start_end[1] - start_end[0];
		make_word_d(data, prompt, str, start_end);
		if (flag)
			return (NO_SPACE);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Procesa un token de comillas dobles en el input.
	- Añade token de apertura '"'.
	- Extrae y tokeniza la palabra dentro de las comillas.
	- Añade token de cierre '"'.
	- Añade token NO_SPACE si la palabra termina pegada a un símbolo.
*/

void	is_double_quote(t_shell *data, t_prompt *promp, const char *str, int *i)
{
	int	status;

	if (str[*i] == '\"')
	{
		add_token(data, promp, "\"", DOUBLE_QUOTE);
		(*i)++;
		status = is_special_word_d(data, promp, str, i);
		if (status == TRUE || status == NO_SPACE)
		{
			add_token(data, promp, "\"", DOUBLE_QUOTE);
			(*i)++;
			if (status == NO_SPACE)
				add_token(data, promp, "", NO_SPACE);
		}
	}
}
