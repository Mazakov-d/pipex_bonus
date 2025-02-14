/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:56:32 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/14 15:06:41 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int fd_is_valid(int fd)
{
    return (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
}

void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs)
	{
		while (strs[i])
		{
			free(strs[i]);
			i++;
		}
		free(strs);
	}
	return (NULL);
}

int	free_cmds_fd(t_cmd *cmds, int pipe_fd[2], int k)
{
	int	i;
	int	j;

	i = 0;
	if (cmds)
	{
		while (cmds && cmds[i].cmd)
		{
			free_strs(cmds[i].cmd);
			free(cmds + i);
		}
	}
	if (fd_is_valid(pipe_fd[0]) == 0)
		close(pipe_fd[0]);
	if (fd_is_valid(pipe_fd[1]) == 0)
		close (pipe_fd[1]);
	return (k);
}

int	free_ptr(void *ptr, int i)
{
	if (ptr)
		free(ptr);
	return (i);
}