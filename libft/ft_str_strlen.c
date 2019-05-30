/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_strlen.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 14:00:07 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/11 14:13:34 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"

inline size_t	ft_strlen(const char *str)
{
	return (ft_strcnlen(str, '\0', (size_t)(-1)));
}

inline size_t	ft_strclen(const char *str, char chr)
{
	return (ft_strcnlen(str, chr, (size_t)(-1)));
}

inline size_t	ft_strnlen(const char *str, size_t max_len)
{
	return (ft_strcnlen(str, '\0', max_len));
}

size_t			ft_strcnlen(const char *str, char chr, size_t max_len)
{
	size_t	len;

	len = 0;
	while ((str[len]) && (str[len] != chr) && (len < max_len))
	{
		++len;
	}
	return (len);
}
