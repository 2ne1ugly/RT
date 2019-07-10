/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 21:07:26 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:35:06 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "../include/rt.h"

char		*read_file(char *buf, int siz, const char *path)
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

void		*read_file_(void *t_)
{
	t_tot	*t;

	t = t_;
	*(t->ptr) = read_file(t->buf, t->siz, t->path);
	return (NULL);
}

void		load_files(t_rt *rt, char **fshader_src, const char *path)
{
	pthread_t	ts[2];

	fshader_src[0] = RT_GLSL_VERSION "\n\n";
	fshader_src[1] = "#define SCENE \\ \n";
	fshader_src[3] = "\n";
	pthread_create(&ts[0], 0, read_file_,
		&(t_tot){rt->scene_buffer, RT_SCENE_SIZE, path, &fshader_src[2]});
	pthread_create(&ts[0], 0, read_file_,
		&(t_tot){rt->buffer, RT_BUFFER_SIZE, RT_FS0, &fshader_src[4]});
	pthread_join(ts[0], 0);
	pthread_join(ts[1], 0);
}

void		load_scene(t_rt *rt, const char *path)
{
	GLint				loglen;
	const char *const	ptr = rt->buffer;
	char				*fshader_src[5];

	load_files(rt, fshader_src, path);
	concat(rt->buffer, fshader_src);
	rt->gldata.fs0_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(rt->gldata.fs0_id, 1, &ptr, 0);
	glCompileShader(rt->gldata.fs0_id);
	glGetShaderiv(rt->gldata.fs0_id, GL_COMPILE_STATUS, (GLint[]){0});
	glGetShaderiv(rt->gldata.fs0_id, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
		panic("scene was corrupt");
}
