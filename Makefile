# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/29 15:17:56 by arherrer          #+#    #+#              #
#    Updated: 2019/07/10 16:32:41 by mchi             ###   ########.fr        #
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
	-L ./libft/ -lft \
	-L ./lib/libpng-1.6.37/ -lpng
name := RT
src := $(addprefix ./src/, callback.c init.c load_glsl.c load_scene.c loop.c main.c make_window.c load_skybox.c post_process.c images.c load_texture.c key_callback.c)
obj := $(src:.c=.o)

.PHONY: all clean fclean re
all:
	@cd ./lib/glfw-3.2.1/ && cmake .
	@cd ../../
	@cd ./lib/libpng-1.6.37/ && cmake .
	@cd ../../
	@make -C ./lib/libpng-1.6.37/
	@make -C ./lib/glfw-3.2.1/
	@make -C ./lib/glew-2.1.0/
	@make -C ./libft/
	@make $(name)
	@install_name_tool -add_rpath @executable_path/lib/libpng-1.6.37/ RT 2> /dev/null ||:

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
	@make -C ./lib/libpng-1.6.37/ clean
	@make -C ./libft fclean

lib_re:
	@cd ./lib/glfw-3.2.1/ && cmake .
	@cd ../../
	@cd ./lib/libpng-1.6.37/ && cmake .
	@cd ../../
	@make -C ./lib/libpng-1.6.37/
	@make -C ./lib/glfw-3.2.1/
	@make -C ./lib/glew-2.1.0/
	@make -C ./libft re
