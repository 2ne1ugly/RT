/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_strjoin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 11:55:42 by mchi              #+#    #+#             */
/*   Updated: 2019/06/01 20:34:39 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strcpy(char *destination, const char *source)
{
	char	*target;

	target = destination;
	while (*source != '\0')
	{
		*target = *source;
		target++;
		source++;
	}
	*target = *source;
	return (destination);
}

char	*ft_strcat(char *destination, const char *source)
{
	char *target;

	target = destination;
	while (*target != '\0')
	{
		target++;
	}
	ft_strcpy(target, source);
	return (destination);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char *new_string;

	new_string = (char *)malloc(sizeof(char) *
		(ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_string == NULL)
	{
		return (NULL);
	}
	ft_strcpy(new_string, s1);
	ft_strcat(new_string, s2);
	return (new_string);
}
