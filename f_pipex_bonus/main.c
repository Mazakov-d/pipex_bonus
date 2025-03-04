/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 18:00:49 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipex(t_cmd **cmds, char **args, int *i)
{
	*cmds = get_commands(args);
	if (!*cmds)
	{
		write(2, "Error: Creating command strings\n", 32);
		return (1);
	}
	*i = last_cmd(*cmds) + 1;
	return (0);
}

int	handle_single_cmd(t_cmd *cmds, char **env, char *infile, char *outfile)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
	{
		free_cmd(cmds);
		return (1);
	}
	if (pid == 0)
		one_cmd(cmds, env, infile, outfile);
	waitpid(pid, &status, 0);
	free_cmd(cmds);
	return (0);
}

int	handle_first_cmd(t_cmd *cmds, char **env, int *fd, char *infile)
{
	if (pipe(fd) < 0)
	{
		write(2, "Error: Creating pipe\n", 21);
		return (free_cmd_int(cmds));
	}
	cmd_infile(cmds->cmd, env, infile, fd);
	return (0);
}

int	execute_middle_cmds(t_cmd *start, t_cmd *end, char **env)
{
	t_cmd	*curr;
	int		pipe_fd[2];
	int		prev_fd[2];

	curr = start;
	prev_fd[0] = start->pipe_in[0];
	prev_fd[1] = start->pipe_in[1];
	
	while (curr != end)
	{
		if (pipe(pipe_fd) < 0)
		{
			close(prev_fd[0]);
			close(prev_fd[1]);
			return (1);
		}
		if (cmd_to_pipe(curr->cmd, env, prev_fd, pipe_fd) == 1)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (1);
		}
		curr->pipe_out[0] = pipe_fd[0];
		curr->pipe_out[1] = pipe_fd[1];
		if (curr->next)
		{
			curr->next->pipe_in[0] = pipe_fd[0];
			curr->next->pipe_in[1] = pipe_fd[1];
		}
		prev_fd[0] = pipe_fd[0];
		prev_fd[1] = pipe_fd[1];
		curr = curr->next;
	}
	return (0);
}

int	process_middle_cmds(t_cmd **cmds, char **env, int *i, int pipe_fd[2])
{
	int	prev_pipe[2];
	int	j;
	t_cmd *current;

	j = *i;
	current = *cmds;
	current->pipe_in[0] = pipe_fd[0];
	current->pipe_in[1] = pipe_fd[1];
	
	while (current->next && --j > 1)
	{
		current = current->next;
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
		if (pipe(pipe_fd) < 0)
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
			return (free_cmd_int(*cmds));
		}
		current->pipe_in[0] = prev_pipe[0];
		current->pipe_in[1] = prev_pipe[1];
		if (cmd_to_pipe(current->cmd, env, prev_pipe, pipe_fd) == 1)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (free_cmd_int(*cmds));
		}
	}
	*cmds = current;
	return (0);
}

void	wait_all_children(void)
{
	int status;
	
	while (waitpid(-1, &status, 0) > 0)
		;
}

int	pipex(char **args, char **env, char *outfile, char *infile)
{
	t_cmd	*cmds;
	t_cmd	*head;
	int		pipe_fd[2];
	int		i;

	if (init_pipex(&cmds, args, &i) == 1)
		return (1);
	head = cmds;
	if (i == 1)
		return (handle_single_cmd(cmds, env, infile, outfile));
	if (handle_first_cmd(cmds, env, pipe_fd, infile) != 0)
		return (1);
	if (process_middle_cmds(&cmds, env, &i, pipe_fd) != 0)
		return (1);
	cmds = cmds->next;
	if (cmd_outfile(cmds->cmd, env, outfile, pipe_fd) == 1)
	{
		free_cmd(head);
		return (1);
	}
	wait_all_children();
	free_cmd(head);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int	save;

	save = 0;
	if (ac < 5)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		write(2, "Usage: ./pipex infile cmd1 cmd2 ... outfile\n", 45);
		write(2, "   or: ./pipex here_doc LIMITER cmd cmd ... outfile\n", 52);
		return (1);
	}
	if (!env)
	{
		write(2, "Error: No environment\n", 22);
		return (1);
	}
	if (is_here_doc(av[1]) != 0)
	{
		save = here_doc(av + 1);
		if (save > 0)
		{
			pipex(av + 2, env, av[ac - 1], ".temp_here_doc.txt");
			unlink(".temp_here_doc.txt");
			return (0);
		}
		else if (save == -1)
			write(2, "Error: here_doc failed\n", 23);
	}
	else
		pipex(av + 1, env, av[ac - 1], av[1]);
	return (0);
}
