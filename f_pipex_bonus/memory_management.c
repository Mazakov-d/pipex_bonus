/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:20:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:41:25 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	*free_str_array(char **strs)
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

void	*free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;
	t_cmd	*head;

	if (!cmd_list)
		return (NULL);
	head = cmd_list;
	while (head && head->prev)
		head = head->prev;
	current = head;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_str_array(current->args);
		free(current);
		current = next;
	}
	return (NULL);
}

void	free_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes || pipe_count <= 0)
		return;
	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	if (!pipes || pipe_count <= 0)
		return;
	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
		{
			if (pipes[i][0] >= 0)
				close(pipes[i][0]);
			if (pipes[i][1] >= 0)
				close(pipes[i][1]);
		}
		i++;
	}
}

int	clean_exit(t_data *data, int ret_val, char *error_msg)
{
	if (error_msg)
		print_error(error_msg);
	if (data->fd.in_file > 0)
	{
		close(data->fd.in_file);
		data->fd.in_file = -1;
	}
	if (data->fd.out_file > 0)
	{
		close(data->fd.out_file);
		data->fd.out_file = -1;
	}
	if (data->fd.pipes)
	{
		close_all_pipes(data->fd.pipes, data->fd.pipe_count);
		free_pipes(data->fd.pipes, data->fd.pipe_count);
		data->fd.pipes = NULL;
	}
	free_cmd_list(data->cmd_list);
	data->cmd_list = NULL;
	return (ret_val);
}

int	free_ptr_with_error(void *ptr, int ret_val, char *error_msg)
{
	if (ptr)
		free(ptr);
	if (error_msg)
		print_error(error_msg);
	return (ret_val);
}