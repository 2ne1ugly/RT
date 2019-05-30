# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arherrer <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/29 15:17:56 by arherrer          #+#    #+#              #
#    Updated: 2019/05/24 21:33:40 by arherrer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS := -Wall -Werror -Wextra -Wpedantic -g
name := libft.a
src := ft_io_putstr.c ft_str_strlen.c
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
