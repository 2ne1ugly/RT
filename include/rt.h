/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 15:14:10 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/25 20:35:27 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

/*
********************************************************************************
** @libraries
********************************************************************************
*/

# include <errno.h>
# include <fcntl.h>
# include <float.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "../lib/glew-2.1.0/include/GL/glew.h"
# include "../lib/glfw-3.2.1/include/GLFW/glfw3.h"
# include "../libft/libft.h"

# define RT_RETINA

# define RT_WIDTH 640
# define RT_HEIGHT 360
# define RT_TITLE "RT"

/*
********************************************************************************
** @data structures
********************************************************************************
*/

# define RT_VS "./shaders/rtv.glsl"
# define RT_FS0 "./shaders/rtf0.glsl"

typedef struct	s_gldata
{
	GLFWwindow	*window;
	GLuint		vertex_array_id;
	GLuint		vertex_buffer;
	GLint		vs_id;
	GLint		fs0_id;
	GLint		program_id;
	GLint		ray_origin_id;
	GLint		rot_id;
	GLint		resolution_id;
	GLint		time_id;
	GLint		mouse_id;
	bool		fs0;
}				t_gldata;

typedef struct	s_vec2
{
	float		x;
	float		y;
}				t_vec2;

typedef struct	s_vec3
{
	float		x;
	float		y;
	float		z;
}				t_vec3;

typedef struct	s_vec4
{
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vec4;

typedef struct	s_material
{
	t_vec4		ambient;
	t_vec4		diffuse;
	t_vec4		specular;
	t_vec4		emission;
}				t_material;

typedef struct	s_object
{
	t_material	material;
	uint16_t	shininess;
	uint16_t	object_type;
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		vertex_0;
	t_vec3		vertex_1;
	t_vec3		vertex_2;
}				t_object;

# define RT_MAX_OBJECTS 512

typedef struct	s_objects
{
	t_object	objects[RT_MAX_OBJECTS];
	uint16_t	object_count;
}				t_objects;

typedef struct	s_uniforms
{
	t_vec3		ray_origin;
	t_vec2		rot;
	t_vec2		resolution;
	t_vec2		time;
	float		delta_time;
	t_vec2		frame;
	t_vec4		mouse;
	bool		entered;
}				t_uniforms;

# define RT_GLSL_VERSION "#version 410"
# define RT_SCENE_SIZE 4096
# define RT_BUFFER_SIZE 65535

typedef struct	s_rt
{
	char		scene_buffer[RT_SCENE_SIZE];
	char		buffer[RT_BUFFER_SIZE];
	t_gldata	gldata;
	t_objects	objects;
	t_uniforms	uniforms;
}				t_rt;

/*
********************************************************************************
** @functions
********************************************************************************
*/

void			load_scene(t_rt *rt, const char *path);
t_rt			*init(t_rt *rt, const char *path);
void			loop(t_rt *rt);
void			panic(const char *msg);

void			make_window(t_gldata *gldata);
void			load_glsl(char *buf, t_gldata *gldata);
void			init_callbacks(t_rt *rt);

#endif
