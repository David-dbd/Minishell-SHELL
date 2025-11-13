/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_multi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:25:13 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/04 16:47:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

void	ft_free_multi(int counter, ...)
{
	int		i;
	va_list	args;
	void	**pointer_address;

	if (counter <= 0)
		return ;
	i = 0;
	va_start(args, counter);
	while (i < counter)
	{
		pointer_address = va_arg(args, void **);
		if (pointer_address != NULL && *pointer_address != NULL)
		{
			free(*pointer_address);
			*pointer_address = NULL;
		}
		i++;
	}
	va_end(args);
}
