/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:09:29 by dmazari           #+#    #+#             */
/*   Updated: 2025/03/07 20:51:49 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_close_fd(char *str, int pipe_fd[2], int i)
{
	if (str)
		free(str);
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	return (i);
}

void	*free_cmd(t_data *cmd)
{
	free_strs(cmd->cmd_a);
	free_strs(cmd->cmd_b);
	return (NULL);
}