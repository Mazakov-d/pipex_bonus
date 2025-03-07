/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:20:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:21:49 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Free a string array
 * 
 * @param strs Array of strings to free
 * @return void* Always returns NULL for convenience
 */
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

/**
 * @brief Free a command list
 * 
 * @param cmd_list Command list to free
 * @return void* Always returns NULL for convenience
 */
void	*free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;
	t_cmd	*head;

	if (!cmd_list)
		return (NULL);
	
	// Find the head of the list
	head = cmd_list;
	while (head && head->prev)
		head = head->prev;
	
	// Free from the head
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

/**
 * @brief Free pipe file descriptors
 * 
 * @param pipes Array of pipe file descriptors
 * @param pipe_count Number of pipes
 */
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

/**
 * @brief Close all pipe file descriptors
 * 
 * @param pipes Array of pipe file descriptors
 * @param pipe_count Number of pipes
 */
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
			// Check if file descriptors are valid before closing
			if (pipes[i][0] >= 0)
				close(pipes[i][0]);
			if (pipes[i][1] >= 0)
				close(pipes[i][1]);
		}
		i++;
	}
}

/**
 * @brief Free the data structure and return an error code
 * 
 * @param data Main data structure
 * @param ret_val Return value
 * @param error_msg Error message (can be NULL)
 * @return int Return value
 */
int	clean_exit(t_data *data, int ret_val, char *error_msg)
{
	if (error_msg)
		print_error(error_msg);
	
	// Close file descriptors
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
	
	// Close and free pipes
	if (data->fd.pipes)
	{
		close_all_pipes(data->fd.pipes, data->fd.pipe_count);
		free_pipes(data->fd.pipes, data->fd.pipe_count);
		data->fd.pipes = NULL;
	}
	
	// Free command list
	free_cmd_list(data->cmd_list);
	data->cmd_list = NULL;
	
	return (ret_val);
}

/**
 * @brief Free a pointer and return an error code
 * 
 * @param ptr Pointer to free
 * @param ret_val Return value
 * @param error_msg Error message (can be NULL)
 * @return int Return value
 */
int	free_ptr_with_error(void *ptr, int ret_val, char *error_msg)
{
	if (ptr)
		free(ptr);
	
	if (error_msg)
		print_error(error_msg);
	
	return (ret_val);
}