/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arherrer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:37:48 by arherrer          #+#    #+#             */
/*   Updated: 2019/05/24 20:27:37 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	glfw_init(t_gldata *gldata)
{
	if (!glfwInit())
		panic("GLFW intialization failed");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	gldata->window = glfwCreateWindow(RT_WIDTH, RT_HEIGHT, RT_TITLE, 0, 0);
	if (!gldata->window)
		panic("GLFW window creation failed");
	glfwMakeContextCurrent(gldata->window);
	glfwSetInputMode(gldata->window, GLFW_STICKY_KEYS, GL_TRUE);
}

static void	glew_init(void)
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		panic("GLEW intialization failed");
}

static void	gl_init(t_gldata *gldata)
{
	static const GLfloat	vertices[] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	glGenVertexArrays(1, &(gldata->vertex_array_id));
	glBindVertexArray(gldata->vertex_array_id);
	glGenBuffers(1, &(gldata->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, gldata->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void		make_window(t_gldata *gldata)
{
	glfw_init(gldata);
	glew_init();
	gl_init(gldata);
}
