/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:09:29 by dmazari           #+#    #+#             */
/*   Updated: 2025/02/26 15:08:43 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_close_fd(char *str, int pipe_fd[2], int i)
{
	if (str)
		free(str);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (i);
}
