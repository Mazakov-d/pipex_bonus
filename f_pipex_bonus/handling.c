/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:39:38 by dmazari           #+#    #+#             */
/*   Updated: 2025/03/07 17:39:50 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipex(t_cmd **cmds, char **args)
{
	*cmds = get_commands(args);
	if (!*cmds)
		return (free_ptr(NULL, 1, "Error: Failed to parse commands\n"));
	return (0);
}

int	handle_first_cmd(t_cmd *cmds, char **env, int pipe_fd[2], char *infile)
{
	if (pipe(pipe_fd) == -1)
		return (free_cmd_fd(cmds, NULL, 1, "Error: pipe\n"));
	
	if (cmd_infile(cmds->cmd, env, infile, pipe_fd) == 1)
		return (free_cmd_fd(cmds, pipe_fd, 1, NULL));
	
	return (0);
}

int	pipe_fail(int pipe_fd[2], int next_pipe[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(next_pipe[0]);
	close(next_pipe[1]);
	return (1);
}

int	process_one_mid_cmd(t_cmd *current, char **env, int pipe_fd[2],
						int next_pipe[2])
{
	if (pipe(next_pipe) == -1)
		return (1);
	
	if (cmd_to_pipe(current->cmd, env, pipe_fd, next_pipe) == 1)
		return (pipe_fail(pipe_fd, next_pipe));
	
	return (0);
}

int	process_middle_cmds(t_cmd **cmds, char **env, int pipe_fd[2])
{
	t_cmd	*current;
	int		next_pipe[2];
	
	current = (*cmds)->next;
	while (current && current->next)
	{
		if (process_one_mid_cmd(current, env, pipe_fd, next_pipe) == 1)
			return (1);
		
		pipe_fd[0] = next_pipe[0];
		pipe_fd[1] = next_pipe[1];
		
		current = current->next;
	}
	
	*cmds = current;
	return (0);
}