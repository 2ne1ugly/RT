# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/29 15:17:56 by arherrer          #+#    #+#              #
#    Updated: 2019/06/26 23:24:52 by mchi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := clang
CFLAGS := -Wall -Werror -Wextra -Wpedantic -g
LDFLAGS = $(FRAMEWORKS) $(LIBS)
FRAMEWORKS := -framework Cocoa -framework CoreVideo -framework IOKit \
	-framework OpenGL
LIBS := \
	-L ./lib/glfw-3.2.1/src/ -lglfw3 \
	-L ./lib/glew-2.1.0/lib/ ./lib/glew-2.1.0/lib/libGLEW.a \
	-L ./libft/ -lft
name := RT
src := $(addprefix ./src/, callback.c init.c load_glsl.c load_scene.c loop.c main.c make_window.c load_skybox.c post_process.c load_noise.c)
obj := $(src:.c=.o)

.PHONY: all clean fclean re
all:
	@cd ./lib/glfw-3.2.1/ && cmake .
	@cd ../../
	@make -C ./lib/glfw-3.2.1/
	@make -C ./lib/glew-2.1.0/
	@make -C ./libft/
	@make $(name)

$(name): $(obj)

	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean: lib_clean
	rm -f $(obj)

fclean: lib_fclean clean
	rm -f $(name)

sre:
	rm -f $(obj)
	rm -f $(name)
	make $(name)

re: lib_re fclean all

.PHONY: lib_clean lib_fclean lib_re
lib_clean:
	@make -C ./libft clean

lib_fclean:
	@make -C ./lib/glfw-3.2.1/ clean
	@make -C ./lib/glew-2.1.0/ clean
	@make -C ./libft fclean

lib_re:
	@cd ./lib/glfw-3.2.1/ && cmake .
	@cd ../../
	@make -C ./lib/glfw-3.2.1/
	@make -C ./lib/glew-2.1.0/
	@make -C ./libft re
