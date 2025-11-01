/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_word.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:34:10 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Procesa un carácter '\' dentro de una palabra.
	- Doble '\\' se convierte en uno solo.
	- Carácter normal se copia tal cual.
	- Un solo '\' al final se omite.
*/

static void	process_slash_char(char *word, char *clean_word, int *j, int *k)
{
	if (word[*j] == '\\' && word[*j + 1] == '\\')
	{
		clean_word[(*k)++] = '\\';
		*j += 2;
	}
	else if (word[*j] != '\\')
		clean_word[(*k)++] = word[(*j)++];
	else
		(*j)++;
}

/*
	Elimina '\' innecesarios de una palabra.
	- Reserva una nueva cadena limpia, copia los caracteres válidos,
	  libera la original y devuelve la nueva.
*/

char	*cleanner_slash(t_shell *data, char *word, int len, char slash)
{
	int		j;
	int		k;
	char	*clean_word;

	j = 0;
	k = 0;
	if (ft_strchr(word, slash))
	{
		clean_word = ft_calloc(len + 1, sizeof(char));
		if (!clean_word)
		{
			free(word);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		while (word[j])
			process_slash_char(word, clean_word, &j, &k);
		clean_word[k] = '\0';
		free(word);
		return (clean_word);
	}
	return (word);
}

/*
	Comprueba si el carácter termina una palabra o marca flag.
	- Devuelve 1 si es operador o símbolo especial.
	- Avanza índice si '\' escapa un carácter.
	- Marca flag si encuentra comillas o '$' para manejar NO_SPACE.
*/

static int	ft_bash_w(const char *str, int *i, int *flag)
{
	char	c;

	c = str[*i];
	if (c == '<' || c == '>' || c == '&' || c == '|'
		|| c == '(' || c == ')' || c == '#'
		|| c == '*' || c == '`' || c == ';')
		return (1);
	if (str[*i] == '\\' && str[*i + 1] != '\0')
	{
		(*i)++;
		return (0);
	}
	if (str[*i] == '\'' || str[*i] == '\"' || str[*i] == '$')
	{
		*flag = TRUE;
		return (1);
	}
	return (0);
}

/*
	Procesa una palabra completa del input.
	- Recorre hasta espacio o símbolo que termina la palabra.
	- Limpia '\' con cleanner_slash.
	- Crea token WORD y detecta si requiere NO_SPACE antes del siguiente token.
*/
void	is_word(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	int		flag;
	int		len;
	int		start;
	char	*word;
	int		token_id;

	flag = FALSE;
	start = *i;
	while (str[*i] != '\0' && !ft_isspace(str[*i]) && !ft_bash_w(str, i, &flag))
		(*i)++;
	len = *i - start;
	if (len > 0)
	{
		word = ft_substr(str, start, len);
		if (!word)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		word = cleanner_slash(data, word, len, '\\');
		token_id = add_token(data, prompt, word, WORD);
		is_cmd(data, &data->prompt, &prompt->tokens[token_id], word);
		if (flag == TRUE)
			add_token(data, prompt, "", NO_SPACE);
	}
}
