/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_multi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:11:15 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/02 21:27:39 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	validate_and_calculate_length(int count, va_list args, size_t *total_len)
{
	int		i;
	char	*current;
	size_t	current_len;

	i = 0;
	*total_len = 0;
	while (i < count)
	{
		current = va_arg(args, char *);
		if (!current)
			return (0);
		current_len = ft_strlen(current);
		if (*total_len > SIZE_MAX - current_len)
			return (0);
		*total_len += current_len;
		i++;
	}
	return (1);
}

int	concatenate_strings(int count, va_list args, char *result, size_t total_len)
{
	int		i;
	char	*current;

	i = 0;
	while (i < count)
	{
		current = va_arg(args, char *);
		if (!current)
			return (0);
		ft_strlcat(result, current, total_len + 1);
		i++;
	}
	return (1);
}

char	*ft_strjoin_multi(int counter, ...)
{
	va_list	args;
	char	*result;
	size_t	total_len;

	if (counter <= 0)
		return (NULL);
	va_start(args, counter);
	if (!validate_and_calculate_length(counter, args, &total_len))
		return (va_end(args), NULL);
	va_end(args);
	if (total_len == 0)
		return (ft_strdup(""));
	result = (char *)malloc(total_len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	va_start(args, counter);
	if (!concatenate_strings(counter, args, result, total_len))
		return (va_end(args), free(result), NULL);
	va_end(args);
	return (result);
}
