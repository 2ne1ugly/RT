/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_glsl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:40:31 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/24 21:19:55 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	load_vshader(char *buf,
					GLint *shader_id,
					const char *path)
{
	const GLchar *const	ptr = buf;
	size_t				cnt;
	int					fd;
	GLint				loglen;

	*shader_id = glCreateShader(GL_VERTEX_SHADER);
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
		panic("vector shader was corrupt");
	if (close(fd) == -1)
		panic(strerror(errno));
}

static void	link_program(t_gldata *gldata)
{
	GLint	loglen;

	gldata->program_id = glCreateProgram();
	glAttachShader(gldata->program_id, gldata->vs_id);
	glAttachShader(gldata->program_id, gldata->fs0_id);
	glLinkProgram(gldata->program_id);
	glGetProgramiv(gldata->program_id, GL_LINK_STATUS, (GLint[]){0});
	glGetProgramiv(gldata->program_id, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
		panic("GLSL program was corrupt");
	glDetachShader(gldata->program_id, gldata->vs_id);
	glDeleteShader(gldata->vs_id);
	glDetachShader(gldata->program_id, gldata->fs0_id);
	glDeleteShader(gldata->fs0_id);
}

void		load_glsl(char *buf, t_gldata *gldata)
{
	load_vshader(buf, &gldata->vs_id, RT_VS);
	link_program(gldata);
	glUseProgram(gldata->program_id);
}
