/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 21:07:26 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/24 21:22:20 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static char	*read_file(char *buf, int siz, const char *path)
{
	int	cnt;
	int	fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		panic(strerror(errno));
	if ((cnt = read(fd, buf, siz)) <= 0)
		panic("scene was empty");
	buf[cnt] = '\0';
	close(fd);
	return (buf);
}

static char	*concat(char *buf, char *const src[5])
{
	int	cnt;
	int	fd;
	int	i;

	fd = open("./shaders/tmp.glsl", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	write(fd, src[0], ft_strlen(src[0]));
	write(fd, src[1], ft_strlen(src[1]));
	i = -1;
	while (src[2][++i])
		(src[2][i] = src[2][i] == '\n' ? ' ' : src[2][i]);
	write(fd, src[2], ft_strlen(src[2]));
	write(fd, src[3], ft_strlen(src[3]));
	write(fd, src[4], ft_strlen(src[4]));
	if (close(fd) == -1)
		panic(strerror(errno));
	fd = open("./shaders/tmp.glsl", O_RDONLY);
	if ((cnt = read(fd, buf, RT_BUFFER_SIZE)) <= 0)
		panic("could not load scene");
	buf[cnt] = '\0';
	if (close(fd) == -1)
		panic(strerror(errno));
	return (buf);
}

void		load_scene(t_rt *rt, const char *path)
{
	GLint				loglen;
	const char *const	ptr = rt->buffer;
	char *const			fshader_src[] = {
		RT_GLSL_VERSION "\n\n",
		"#define SCENE \\ \n",
		read_file(rt->scene_buffer, RT_SCENE_SIZE, path),
		"\n",
		read_file(rt->buffer, RT_BUFFER_SIZE, RT_FS0),
	};

	concat(rt->buffer, fshader_src);
	rt->gldata.fs0_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(rt->gldata.fs0_id, 1, &ptr, 0);
	glCompileShader(rt->gldata.fs0_id);
	glGetShaderiv(rt->gldata.fs0_id, GL_COMPILE_STATUS, (GLint[]){0});
	glGetShaderiv(rt->gldata.fs0_id, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
		panic("scene was corrupt");
}
