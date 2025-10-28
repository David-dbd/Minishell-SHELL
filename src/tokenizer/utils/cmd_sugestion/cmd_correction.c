/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_correction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:00 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/28 14:22:11 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	is_yes_or_no(const char *str)
{
	if (ft_strcmp(str, "y") == 0 || ft_strcmp(str, "yes") == 0)
		return (YES);
	else if (ft_strcmp(str, "n") == 0 || ft_strcmp(str, "no") == 0)
		return (NO);
	return (ERROR);
}

static	int	parse_answer(t_shell *d, t_token *token, char *str, char *built_in)
{
	int	result;
	//funciones pequeñas para reconocer yes/y no/n
	result = is_yes_or_no(str);
	if (result == YES)
	{
		free (token->value);
		token->value = ft_strdup(built_in);
		if (!token->value)
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		return (SUCCESS);
	}
	else if (result == FALSE)
		return (FALSE);
	return (ERROR); //El usuario introduce cualquier otro valor distinto a yes/no
}

static int	ask_confirmation(t_shell *data, t_token *token, char *built_in)
{
	char	*ptr;

	while (1)
	{
		printf("\033[1;32mDid you mean %s? y/n\033[0m\n", built_in);
		ptr = ic_readline("\033[1;32m->\033[0m");//desde aqui hasta el else de readline puede ser una sola funcion que se llame sola
		if (!ptr)
			exit_error(data, NULL, EXIT_FATAL_SIGNAL);
		if (check_signals(data, NULL, NULL, NULL))
		{
			free (ptr);
			return (FAILURE);
		}
		if (parse_answer(data, token, ptr, built_in) != ERROR)
		{
			free (ptr);
			return (SUCCESS);
		}
		//Si es ERROR se vuelve a repetir la pregunta
		free (ptr);
	}
	return (SUCCESS);
}

/*
 * Heurística para detectar comandos mal escritos con una diferencia
 * mínima entre cadenas s1 y s2.
 *
 * Se usa para sugerir correcciones cuando el usuario comete errores
 * tipográficos al escribir comandos.
 *
 * Primero se compara la longitud de ambas cadenas. Solo se aceptan si
 * la diferencia es de ±1 carácter.
 *
 * Luego se recorre s2 y se avanza en s1 solo si los caracteres coinciden.
 * Esto permite tolerar errores leves.
 *
 * Al final se evalúa si las cadenas terminan con una diferencia mínima.
 * Si es así, se considera un match.
 *
 * Ejemplos:
 * - Inserción: "echop" vs "echo" -> (s1[j + 1] == '\0' && s2[i] == '\0')
 * - Eliminación: "eho" vs "echo" -> s1[j] == '\0' && s2[i + 1] == '\0'
 * - Reemplazo: "exho" vs "echo" -> j + 1 == len_1 && i == len_2) || (i + 1 == len_2 && j == len_1)
 * transposición
*/

static int	find_match(const char *s1, const char *s2)
{
	int	i;
	int	j;
	int	len_1;
	int	len_2;

	i = 0;
	j = 0;
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	if (!(len_1 - len_2 >= -1 && len_1 - len_2 <= 1))
		return (FALSE);
	while (s2[i] != '\0' && s1[j] != '\0')
	{
		if (s1[j] == s2[i])
			j++;
		i++;
	}
	if ((s1[j] == '\0' && s2[i + 1] == '\0')
		|| (s1[j + 1] == '\0' && s2[i] == '\0')
		|| s1[j] == '\0' && s2[i] == '\0')//si s1 esta contendio en s2 o si son iguales en len o si s1 se pasa por una letra
		return (SUCCESS);
	else if ((j + 1 == len_1 && i == len_2) || (i + 1 == len_2 && j == len_1)) //Seguir logica de abc vs axc
		return (SUCCESS);
	return (FALSE);
}

int	cmd_correction(t_shell *data, t_token *tokens, int n_tokens)
{
	char	*built_in[10];
	int		result;
	int		i;
	int		j;

	i = 0;
	built_in[0] = "cd";
	built_in[1] = "echo";
	built_in[2] = "export";
	built_in[3] = "unset";
	built_in[4] = "env";
	built_in[5] = "exit";
	built_in[6] = "pwd";
	built_in[7] = "alias";
	built_in[8] = "unalias";
	built_in[9] = NULL;
	result = SUCCESS;
	while (i < n_tokens)
	{
		if (tokens[i].type == COMMAND)
		{
			j = 0;
			while (j < 9)
			{
				if (ft_strlen(tokens[i].value) >= 1
					&& find_match(tokens[i].value, built_in[j])) //si solo hay un caracter diferente
				{ //mayor que 1 porque el el built_in mas corto es cd y es de 1
					result = ask_confirmation(data, &tokens[i], built_in[j]);
					break ;//Solo corrige el el primero que encuentra mal escrito en lugar de intnetar corregir todos ya que sería fastidioso para el usuario
				}
				j++;
			}
		}
		i++;
	}
	if (!result)
		clean_prompt(&data->prompt);
	return (result);
}
