/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 13:49:33 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/11 14:13:33 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STR_H
# define FT_STR_H

# include <stddef.h>

/*
** strlen family
*/

size_t	ft_strlen(const char *str);
size_t	ft_strclen(const char *str, char chr);
size_t	ft_strnlen(const char *str, size_t max_len);
size_t	ft_strcnlen(const char *str, char chr, size_t max_len);

#endif
