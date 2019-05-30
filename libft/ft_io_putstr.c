/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_io_putstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 14:07:27 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/11 14:12:30 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_io.h"

inline size_t	ft_putstr(const char *str)
{
	return (ft_fdputstr(str, 1));
}

inline size_t	ft_fdputstr(const char *str, int fd)
{
	return (write(fd, str, ft_strlen(str)));
}
