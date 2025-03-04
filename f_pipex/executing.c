/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:06:35 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 15:02:09 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**cmd(char *c, int a, int b)
{
	char	**cmd;

	cmd = split_c(c, a, b);
	if (!cmd)
		return (NULL);
	return (cmd);
}

int	open_switch_stdin(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: Cannot open infile");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	open_switch_stdout(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error: Cannot open outfile");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	cmd_a_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2])
{
	int			pid;
	t_pipe_data	data;

	pid = fork();
	if (pid < 0)
		return (error_fork(1, NULL, pipe_fd));
	if (pid == 0)
	{
		data.pipe_fd[0] = pipe_fd[0];
		data.pipe_fd[1] = pipe_fd[1];
		data.infile = infile;
		child_process_first(cmd_a, env, data);
	}
	close(pipe_fd[1]);
	return (0);
}

int	cmd_b_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2])
{
	int			pid;
	int			status1;
	int			status2;
	t_pipe_data	data;

	pid = fork();
	if (pid < 0)
		return (error_fork(1, NULL, pipe_fd));
	if (pid == 0)
	{
		data.pipe_fd[0] = pipe_fd[0];
		data.pipe_fd[1] = pipe_fd[1];
		data.outfile = outfile;
		child_process_second(cmd_b, env, data);
	}
	close(pipe_fd[0]);
	waitpid(-1, &status1, 0);
	waitpid(-1, &status2, 0);
	return (0);
}

int	execute_cmd_b(char **cmd_b, char **env, char *path_cmd, int pipe_fd[2])
{
	if (open_switch_stdout(outfile) == 1)
	{
		close(pipe_fd[0]);
		if (path_cmd)
			free(path_cmd);
		exit(EXIT_FAILURE);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	if (path_cmd)
		execve(path_cmd, cmd_b, env);
	perror("Error: Command not found");
	if (path_cmd)
		free(path_cmd);
	exit(EXIT_FAILURE);
	return (0);
}

int	cmd_b_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2])
{
	int		pid;
	int		status1;
	int		status2;
	char	*path_cmd;

	path_cmd = get_path_cmd(cmd_b[0], get_path_env(env));
	pid = fork();
	if (pid < 0)
	{
		if (path_cmd)
			free(path_cmd);
		return (error_fork(1, NULL, pipe_fd));
	}
	if (pid == 0)
	{
		close(pipe_fd[1]);
		execute_cmd_b(cmd_b, env, path_cmd, pipe_fd);
	}
	if (path_cmd)
		free(path_cmd);
	close(pipe_fd[0]);
	waitpid(-1, &status1, 0);
	waitpid(-1, &status2, 0);
	return (0);
}
