# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/29 15:17:56 by arherrer          #+#    #+#              #
#    Updated: 2019/07/10 02:03:48 by mchi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS := -Wall -Werror -Wextra -Wpedantic -g
name := libft.a
src := ft_io_putstr.c ft_str_strlen.c ft_str_strjoin.c ft_memcpy.c
obj := $(src:.c=.o)

.PHONY: all
all: $(name)

$(name): $(obj)
	ar ru $@ $^
	ranlib $@

.PHONY: clean
clean:
	rm -f $(obj)

.PHONY: fclean
fclean: clean
	rm -f $(name)

.PHONY: all
re: fclean all
