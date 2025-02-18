/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:56:32 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/18 18:45:14 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

void	*free_cmd(t_cmd *cmd)
{
	t_cmd	*save;

	while (cmd->prev)
		cmd = cmd->prev;
	while (cmd)
	{
		save = cmd;
		free_strs(cmd->cmd);
		cmd = cmd->next;
		free(save);
	}
	return (NULL);
}

int	free_cmd_int(t_cmd *cmd)
{
	free_cmd(cmd);
	return (1);
}

int	free_ptr(void *ptr, int i)
{
	if (ptr)
		free(ptr);
	return (i);
}

int	free_cmd_fd(t_cmd *cmd, int pipe_fd[2], int i)
{
	free_cmd(cmd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (i);
}
