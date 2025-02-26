/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:56:32 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 16:31:42 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void *free_strs(char **strs)
{
	int i;

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

void *free_cmd(t_cmd *cmd)
{
	t_cmd *save;
	t_cmd *head;

	head = cmd;
	while (head && head->prev)
		head = head->prev;
	cmd = head;
	while (cmd)
	{
		save = cmd->next;
		if (cmd->cmd)
			free_strs(cmd->cmd);
		free(cmd);
	cmd = save;
	}
	return (NULL);
}

int	free_cmd_int(t_cmd *cmd)
{
	free_cmd(cmd);
	return (1);
}

int	free_ptr(void *ptr, int i, char *error)
{
	if (ptr)
		free(ptr);
	if (error)
		write(2, error, ft_strlen(error));
	return (i);
}

int	free_cmd_fd(t_cmd *cmd, int pipe_fd[2], int i, char *error)
{
	if (error)
		write(2, error, ft_strlen(error));
	free_cmd(cmd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (i);
}
