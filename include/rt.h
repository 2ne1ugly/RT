/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 15:14:10 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:43:23 by arherrer         ###   ########.fr       */
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
# include "../lib/libpng-1.6.37/png.h"

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
# define RT_POST_FS "./shaders/postf.glsl"

typedef struct	s_tot
{
	char		*buf;
	size_t		siz;
	const char	*path;
	char		**ptr;
}				t_tot;

typedef struct	s_gldata
{
	GLFWwindow	*window;
	GLuint		vertex_array_id;
	GLuint		vertex_buffer;
	GLuint		vs_id;
	GLuint		fs0_id;
	GLuint		program_id;
	GLint		ray_origin_id;
	GLint		rot_id;
	GLint		aspect_id;
	GLint		time_id;
	GLint		mouse_id;
	GLint		skybox_id;
	GLint		render_sampler;
	GLint		normal_sampler;
	GLint		albedo_sampler;
	GLint		pp_flags_id;
	bool		fs0;
	GLuint		framebuffer_id;
	GLuint		target_texture;
	GLuint		pp_fs_id;
	GLuint		pp_program_id;
	GLuint		normal_map;
	GLuint		albedo_map;

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

typedef struct	s_png
{
	FILE			*fp;
	unsigned int	width;
	unsigned int	height;
	int				color_type;
	int				bit_depth;
	png_structp		png_ptr;
	png_infop		info_ptr;
	int				row_bytes;
	int				interlace_type;
	unsigned char	*data;
	png_bytepp		rows;
}				t_png;

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
	float		aspect;
	t_vec2		time;
	float		delta_time;
	t_vec2		frame;
	t_vec4		mouse;
	bool		entered;
	GLuint		skybox;
	int			save_screenshot;
	int			pp_flag;
}				t_uniforms;

typedef struct	s_image
{
	png_structp	png;
	png_infop	info;
	FILE		*fp;
	png_colorp	palette;
	png_bytepp	rows;
	uint8_t		*pixels;
}				t_image;

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

char			*read_file(char *buf, int siz, const char *path);
void			load_scene(t_rt *rt, const char *path);
GLuint			load_skybox(t_rt *rt);
t_rt			*init(t_rt *rt, const char *path);
void			loop(t_rt *rt);
void			panic(const char *msg);
void			save_image(t_rt *rt);

void			make_window(t_gldata *gldata);
void			load_glsl(char *buf, t_gldata *gldata);
void			init_callbacks(t_rt *rt);
void			load_shader(char *b, GLuint *sid, GLenum st, const char *path);
void			load_noise(t_rt *rt);
void			link_program(GLuint *program_out, GLuint vs_id, GLuint fs_id);
void			init_pp(t_rt *rt);
void			pp_uniform_update(t_rt *rt);

void			parse_png(t_png *png);
void			init_png(t_png *png, const char *path);

void			load_textures(t_rt *rt);

void			dispatch_key_press1(t_uniforms *u, int key);
void			dispatch_key_press2(t_uniforms *u, int key);
#endif
