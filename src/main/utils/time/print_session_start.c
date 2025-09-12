/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:13:28 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/12 17:31:29 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Imprime el título del minishell y muestra la hora exacta en que se inicia la
	sesión. La hora se formatea en color cian e incluye hora, minutos, segundos,
	día, mes y año. También obtiene el loggin del usuario para hacer el output
	de bienevenida mucho mas personalizado, aceptando solamente loggins valido.
	Luego llama a la función print_time_of_day para mostrar un
	mensaje personalizado según el momento del día en que se inicia la sesión.
*/

void	print_minishell_title(void)
{
	printf(TITLE_COLOR "\n\n");
	printf(T1 "\n");
	printf(T2 "\n");
	printf(T3 "\n");
	printf(T4 "\n");
	printf(T5 "\n");
	printf("\n\n" RESET_COLOR);
}

static void check_user_name_syntax(char *user_name, int *validation)
{
	int i;

	i = 0;
	while (user_name[i] != '\0')
	{
		if (!ft_isprint(user_name[i]) || ft_isspace(user_name[i]))
		{
			*validation = FAILURE;
			return ;
		}
		i++;
	}
	*validation = SUCCESS;
}

static char *get_user_name(char **user_name, int *validation)
{
	*user_name = readline("\033[1m\033[1;32mIntroduce your login: \033[0m");
	printf("\n\n");
	if (!*user_name) // Ctrl+D detectado
	{
		*validation = FAILURE;
		return (NULL);
	}
	if (!**user_name) // Cadena vacía
	{
		ft_printf_fd(STDOUT, MSG_INVALID_NAME);
		*validation = FAILURE;
		return (*user_name);
	}
	check_user_name_syntax(*user_name, validation);
	return (*user_name);
}

static char *is_valid_user_name(void)
{
	char	*user_name;
	int		validation;

	user_name = NULL;
	while (1)
	{
		if (!get_user_name(&user_name, &validation))
			return (NULL);
		if (validation == SUCCESS)
			break ;
		ft_printf_fd(STDOUT, MSG_INVALID_NAME);
	}
	return (user_name);
}

void	print_session_start(t_shell *data, time_t start, char *user_name)
{
	struct tm	*local;
	char *temp;

	local = localtime(&start);
	print_minishell_title();
	user_name = is_valid_user_name();
	if (!user_name)
		exit_succes(data, MSG_GOODBYE, EXIT_SUCCESS);
	temp = user_name;
	user_name = ft_strdup(temp);
	if (!user_name)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	free (temp);
	print_time_of_day(start, user_name);
	printf(MSG_TIME_START,
		user_name, local->tm_hour, local->tm_min, local->tm_sec,
		local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}
