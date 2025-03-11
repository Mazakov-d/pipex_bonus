/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:50:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/11 15:52:51 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*get_cmd_path(char *cmd, char **path_dirs)
{
	int		i;
	char	*full_path;
	char	**cmd_parts;

	if (!cmd || !path_dirs)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	cmd_parts = split_c(cmd, ' ', '\0');
	if (!cmd_parts)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		full_path = ft_strcat(path_dirs[i], cmd_parts[0]);
		if (!full_path)
		{
			free_str_array(cmd_parts);
			return (NULL);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_str_array(cmd_parts);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_str_array(cmd_parts);
	return (NULL);
}

void	setup_cmd_pipes(t_data *data, t_cmd *cmd)
{
	int	position;
	int	i;

	position = cmd->position;
	printf("position : %d\n", position);
	if (position == 0)
	{
		if (dup2(data->fd.in_file, STDIN_FILENO) == -1)
			exit(1);
		if (dup2(data->fd.pipes[position][1], STDOUT_FILENO) == -1)
			exit(1);
	}
	else if (position == data->cmd_count - 1)
	{
		if (dup2(data->fd.pipes[position - 1][0], STDIN_FILENO) == -1)
			exit(1);
		if (dup2(data->fd.out_file, STDOUT_FILENO) == -1)
			exit(1);
	}
	else
	{
		if (dup2(data->fd.pipes[position - 1][0], STDIN_FILENO) == -1)
			exit(1);
		if (dup2(data->fd.pipes[position][1], STDOUT_FILENO) == -1)
			exit(1);
	}
	i = 0;
	while (i < data->fd.pipe_count)
	{
		close(data->fd.pipes[i][0]);
		close(data->fd.pipes[i][1]);
		i++;
	}
	if (data->fd.in_file > 0)
		close(data->fd.in_file);
	if (data->fd.out_file > 0)
		close(data->fd.out_file);
}

void	child_process(t_data *data, t_cmd *cmd, char **path_dirs)
{
	char	*cmd_path;

	setup_cmd_pipes(data, cmd);
	cmd_path = get_cmd_path(cmd->args[0], path_dirs);
	if (!cmd_path)
	{
		print_error("Command not found: ");
		print_error(cmd->args[0]);
		print_error("\n");
		exit(127);
	}
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		free(cmd_path);
		print_error("Failed to execute: ");
		print_error(cmd->args[0]);
		print_error("\n");
		exit(126);
	}
}

int	wait_for_children(int cmd_count, pid_t *pids)
{
	int	i;
	int	status;
	int	exit_status;

	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
		}
		i++;
	}
	return (exit_status);
}

int	run_pipeline(t_data *data)
{
	t_cmd	*cmd;
	pid_t	*pids;
	char	**path_dirs;
	int		i;
	int		exit_status;

	path_dirs = get_path_from_env(data->env);
	if (!path_dirs)
		return (clean_exit(data, 1, "Failed to get PATH"));
	pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!pids)
	{
		free_str_array(path_dirs);
		return (clean_exit(data, 1, "Malloc failed for PIDs"));
	}
	cmd = data->cmd_list;
	i = 0;
	while (cmd && i < data->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free_str_array(path_dirs);
			free(pids);
			return (clean_exit(data, 1, "Fork failed"));
		}
		if (pids[i] == 0)
		{
			free(pids);
			child_process(data, cmd, path_dirs);
			exit(1);
		}
		cmd = cmd->next;
		i++;
	}
	i = 0;
	while (i < data->fd.pipe_count)
	{
		close(data->fd.pipes[i][0]);
		close(data->fd.pipes[i][1]);
		i++;
	}
	if (data->fd.in_file > 0)
		close(data->fd.in_file);
	if (data->fd.out_file > 0)
		close(data->fd.out_file);
	exit_status = wait_for_children(data->cmd_count, pids);
	free_str_array(path_dirs);
	free(pids);
	return (exit_status);
}
