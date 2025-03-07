/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:56:32 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:13:49 by dorianmazar      ###   ########.fr       */
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

void	*free_data(t_data *data)
{
	t_data	*save;
	t_data	*head;

	if (!data)
		return (NULL);
	head = data;
	while (head && head->prev)
		head = head->prev;
	data = head;
	while (data)
	{
		save = data->next;
		if (data->cmd)
			free_strs(data->cmd);
		free(data);
		data = save;
	}
	return (NULL);
}

int	free_data_int(t_data *data)
{
	free_data(data);
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

int	free_data_fd(t_data *data, int i, char *error)
{
	int	i;

	i = 0;
	if (error)
		write(2, error, ft_strlen(error));
	while (data->pipe_fd[i])
	{
		close(data->pipe_fd[i][0]);
		close(data->pipe_fd[i][1]);
		i++;
	}
	close(data->fd_in);
	close(data->fd_out);
	free_data(data);
	return (i);
}
