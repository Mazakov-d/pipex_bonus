/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:30:20 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 16:23:24 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process_first(t_cmd *cmd, char **env, t_pipe_data data)
{
	char	*path_cmd;

	close(data.pipe_fd[0]);
	if (open_switch_stdin(data.infile) == 1)
	{
		close(data.pipe_fd[1]);
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	dup2(data.pipe_fd[1], STDOUT_FILENO);
	close(data.pipe_fd[1]);
	path_cmd = get_path_cmd(cmd->cmd_a[0], get_path_env(env));
	if (path_cmd)
	{
		execve(path_cmd, cmd->cmd_a, env);
		free(path_cmd);
	}
	else if (cmd->cmd_a[0] && cmd->cmd_a[0][0] == '/')
		execve(cmd->cmd_a[0], cmd->cmd_a, env);
	free_cmd(cmd);
	write(2, "Error : can't find the command\n", 32);
	exit(EXIT_FAILURE);
}

void	child_process_second(t_cmd *cmd, char **env, t_pipe_data data)
{
	char	*path_cmd;

	close(data.pipe_fd[1]);
	if (open_switch_stdout(data.outfile) == 1)
	{
		close(data.pipe_fd[0]);
		free_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	dup2(data.pipe_fd[0], STDIN_FILENO);
	close(data.pipe_fd[0]);
	path_cmd = get_path_cmd(cmd->cmd_b[0], get_path_env(env));
	if (path_cmd)
	{
		execve(path_cmd, cmd->cmd_b, env);
		free(path_cmd);
	}
	else if (cmd->cmd_b[0] && cmd->cmd_b[0][0] == '/')
		execve(cmd->cmd_b[0], cmd->cmd_b, env);
	free_cmd(cmd);
	write(2, "Error : can't find the command\n", 32);
	exit(EXIT_FAILURE);
}
