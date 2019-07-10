/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_io.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 14:05:14 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:43:58 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IO_H
# define FT_IO_H

# include <unistd.h>

# include "ft_str.h"

/*
** putstr family
*/

size_t	ft_putstr(const char *str);
size_t	ft_fdputstr(const char *str, int fd);
void	*ft_memcpy(void *dst, const void *src, size_t n);
#endif
