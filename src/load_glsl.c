/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_glsl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:40:31 by arherrer          #+#    #+#             */
/*   Updated: 2019/06/20 16:21:12 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

#include <stdio.h>

void	load_shader(char *buf, GLuint *shader_id,
	GLenum shader_type, const char *path)
{
	const GLchar *const	ptr = buf;
	size_t				cnt;
	int					fd;
	GLint				loglen;

	*shader_id = glCreateShader(shader_type);
	if ((fd = open(path, O_RDONLY)) <= 0)
		panic(strerror(errno));
	if ((cnt = read(fd, buf, RT_BUFFER_SIZE)) <= 0)
		panic("vector shader was empty");
	buf[cnt] = '\0';
	glShaderSource(*shader_id, 1, &ptr, 0);
	glCompileShader(*shader_id);
	glGetShaderiv(*shader_id, GL_COMPILE_STATUS, (GLint[]){0});
	glGetShaderiv(*shader_id, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		char str[10000];
		glGetShaderInfoLog(*shader_id, 10000, &loglen, str);
		printf("%s\n", str);
		panic("vector shader was corrupt");
	}
	if (close(fd) == -1)
		panic(strerror(errno));
}

#include <stdio.h>

void		link_program(GLuint *program_out, GLuint vs_id, GLuint fs_id)
{
	GLint	loglen;

	*program_out = glCreateProgram();
	glAttachShader(*program_out, vs_id);
	glAttachShader(*program_out, fs_id);
	glLinkProgram(*program_out);
	glGetProgramiv(*program_out, GL_LINK_STATUS, (GLint[]){0});
	glGetProgramiv(*program_out, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		char str[10000];
		glGetProgramInfoLog(*program_out, 10000, &loglen, str);
		printf("%s", str);
		panic("GLSL program was corrupt");
	}
	glDetachShader(*program_out, vs_id);
	glDetachShader(*program_out, fs_id);
}

void		load_glsl(char *buf, t_gldata *gldata)
{
	load_shader(buf, &gldata->vs_id, GL_VERTEX_SHADER, RT_VS);
	link_program(&gldata->program_id, gldata->vs_id, gldata->fs0_id);
}
