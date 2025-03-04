/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:02:38 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 16:45:15 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	child_process_first(char **cmd_a, char **env, 
						char *infile, int pipe_fd[2])
{
	char	*path_cmd;
	char	**paths;

	close(pipe_fd[0]);
	if (open_switch_stdin(infile) == 1)
	{
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	paths = get_path_env(env);
	path_cmd = get_path_cmd(cmd_a[0], paths);
	if (path_cmd)
	{
		execve(path_cmd, cmd_a, env);
		free(path_cmd);
	}
	else if (cmd_a[0] && (cmd_a[0][0] == '/' || 
		(cmd_a[0][0] == '.' && cmd_a[0][1] == '/')))
		execve(cmd_a[0], cmd_a, env);
	if (paths)
		free_strs(paths);
	perror("Error: Command not found");
	exit(EXIT_FAILURE);
}

int	cmd_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2])
{
	int		pid;

	pid = fork();
	if (pid < 0)
		return (free_ptr(NULL, 1, "Error: fork\n"));
	if (pid == 0)
		child_process_first(cmd_a, env, infile, pipe_fd);
	close(pipe_fd[1]);
	return (0);
}

void	child_process_last(char **cmd_b, char **env, 
						char *outfile, int pipe_fd[2])
{
	char	*path_cmd;
	char	**paths;

	close(pipe_fd[1]);
	if (open_switch_stdout(outfile) == 1)
	{
		close(pipe_fd[0]);
		exit(EXIT_FAILURE);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	paths = get_path_env(env);
	path_cmd = get_path_cmd(cmd_b[0], paths);
	if (path_cmd)
	{
		execve(path_cmd, cmd_b, env);
		free(path_cmd);
	}
	else if (cmd_b[0] && (cmd_b[0][0] == '/' || 
		(cmd_b[0][0] == '.' && cmd_b[0][1] == '/')))
		execve(cmd_b[0], cmd_b, env);
	if (paths)
		free_strs(paths);
	perror("Error: Command not found");
	exit(EXIT_FAILURE);
}

int	cmd_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2])
{
	int		pid;

	pid = fork();
	if (pid < 0)
		return (free_ptr(NULL, 1, "Error: fork\n"));
	if (pid == 0)
		child_process_last(cmd_b, env, outfile, pipe_fd);
	close(pipe_fd[0]);
	waitpid(pid, NULL, 0);
	waitpid(-1, NULL, 0);
	return (0);
}

int	cmd_to_pipe(char **cmd, char **env, int prev[2], int next[2])
{
	int		pid;
	char	**paths;
	char	*path_cmd;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		close(prev[1]);
		close(next[0]);
		dup2(prev[0], STDIN_FILENO);
		dup2(next[1], STDOUT_FILENO);
		close(prev[0]);
		close(next[1]);
		paths = get_path_env(env);
		path_cmd = get_path_cmd(cmd[0], paths);
		if (path_cmd)
			execve(path_cmd, cmd, env);
		if (path_cmd)
			free(path_cmd);
		if (paths)
			free_strs(paths);
		perror("Error: Command not found");
		exit(EXIT_FAILURE);
	}
	close(prev[0]);
	close(prev[1]);
	waitpid(pid, NULL, 0);
	return (0);
}

int	one_cmd(t_cmd *cmd, char **env, char *infile, char *outfile)
{
	char	*path_cmd;
	char	**paths;

	if (open_switch_in_out(infile, outfile) == 1)
		return (free_cmd_int(cmd));
	paths = get_path_env(env);
	path_cmd = get_path_cmd(cmd->cmd[0], paths);
	if (path_cmd)
	{
		execve(path_cmd, cmd->cmd, env);
		free(path_cmd);
	}
	else if (cmd->cmd[0] && (cmd->cmd[0][0] == '/' || 
		(cmd->cmd[0][0] == '.' && cmd->cmd[0][1] == '/')))
		execve(cmd->cmd[0], cmd->cmd, env);
	if (paths)
		free_strs(paths);
	perror("Error: Command not found");
	exit(EXIT_FAILURE);
}
