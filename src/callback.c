/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 20:43:41 by arherrer          #+#    #+#             */
/*   Updated: 2019/07/10 16:29:56 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

static void	rt_cursor_pos_cb(GLFWwindow *window, double xpos, double ypos)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);

	rt->uniforms.mouse.z = rt->uniforms.mouse.x;
	rt->uniforms.mouse.w = rt->uniforms.mouse.y;
	rt->uniforms.mouse.x = xpos;
	rt->uniforms.mouse.y = ypos;
}

static void	rt_cursor_enter_cb(GLFWwindow *window, int entered)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);

	rt->uniforms.entered = entered;
}

static void	rt_key_cb(GLFWwindow *window, int key, int scancode, int action)
{
	t_rt *const	rt = glfwGetWindowUserPointer(window);

	(void)scancode;
	if (action == GLFW_PRESS)
	{
		dispatch_key_press1(&rt->uniforms, key);
		dispatch_key_press2(&rt->uniforms, key);
	}
	if (action == GLFW_REPEAT)
		dispatch_key_press1(&rt->uniforms, key);
}

void		init_callbacks(t_rt *rt)
{
	glfwSetWindowUserPointer(rt->gldata.window, rt);
	glfwSetKeyCallback(rt->gldata.window,
		(void (*)(struct GLFWwindow *, int, int, int, int))rt_key_cb);
	glfwSetCursorPosCallback(rt->gldata.window, rt_cursor_pos_cb);
	glfwSetCursorEnterCallback(rt->gldata.window, rt_cursor_enter_cb);
}
