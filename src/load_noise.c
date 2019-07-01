/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_noise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchi <mchi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 22:41:54 by mchi              #+#    #+#             */
/*   Updated: 2019/06/28 00:27:32 by mchi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

#define NOISE_SIZE 1024
unsigned char	**malloc_noise()
{
	int i;
	unsigned char **noise;

	noise = malloc(sizeof(unsigned char *) * NOISE_SIZE);
	i = 0;
	while (i < NOISE_SIZE)
	{
		noise[i] = malloc(sizeof(unsigned char) * NOISE_SIZE * 4);
		i++;
	}
	return (noise);
}

void			set_noise(unsigned char **noise)
{
	int i;
	int j;
	int k;

	i = 0;
	while (i < NOISE_SIZE)
	{
		j = 0;
		while (j < NOISE_SIZE)
		{
			k = 0;
			while (k < 4)
			{
				noise[i][j * 4 + k] = rand() % 255;
				k++;
			}
			j++;
		}
		i++;
	}
}

void			free_noise(unsigned char **noise)
{
	int i;

	i = 0;
	while (i < NOISE_SIZE)
	{
		free(noise[i]);
		i++;
	}
	free(noise);
	
}

void			load_noise(t_rt *rt)
{
	unsigned char	**noise;

	noise = malloc_noise();
	set_noise(noise);
	glGenTextures(1, &rt->uniforms.noise);
	glBindTexture(GL_TEXTURE_2D, rt->uniforms.noise);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		NOISE_SIZE, NOISE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, noise);
	free_noise(noise);
}
