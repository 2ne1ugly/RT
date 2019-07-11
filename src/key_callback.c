/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_callback.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 16:30:07 by mchi              #+#    #+#             */
/*   Updated: 2019/07/11 11:46:38 by arherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

void		dispatch_key_press1(t_uniforms *u, int key)
{
	if (key == GLFW_KEY_W)
		u->ray_origin.z -= 8 * u->delta_time;
	if (key == GLFW_KEY_S)
		u->ray_origin.z += 8 * u->delta_time;
	if (key == GLFW_KEY_A)
		u->ray_origin.x -= 8 * u->delta_time;
	if (key == GLFW_KEY_D)
		u->ray_origin.x += 8 * u->delta_time;
	if (key == GLFW_KEY_J)
		u->ray_origin.y += 8 * u->delta_time;
	if (key == GLFW_KEY_K)
		u->ray_origin.y -= 8 * u->delta_time;
	if (key == GLFW_KEY_Q)
		u->rot.x -= 8 * u->delta_time;
	if (key == GLFW_KEY_E)
		u->rot.x += 8 * u->delta_time;
	if (key == GLFW_KEY_H)
		u->rot.y -= 8 * u->delta_time;
	if (key == GLFW_KEY_L)
		u->rot.y += 8 * u->delta_time;
}

void		dispatch_key_press2(t_uniforms *u, int key)
{
	if (key == GLFW_KEY_1)
		u->pp_flag ^= 1;
	if (key == GLFW_KEY_2)
		u->pp_flag ^= 2;
	if (key == GLFW_KEY_3)
		u->pp_flag ^= 4;
	if (key == GLFW_KEY_P)
		u->save_screenshot = 1;
	if (key == GLFW_KEY_SPACE)
	{
		u->ray_origin = (t_vec3){0, 1, 8};
		u->rot = (t_vec2){0, 0};
	}
}
