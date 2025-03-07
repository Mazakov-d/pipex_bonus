/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 17:40:08 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	wait_all_children(void)
{
	int status;
	
	while (waitpid(-1, &status, 0) > 0)
		;
}

int	create_empty_outfile(char *outfile)
{
	int fd;
	
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		close(fd);
		return (0);
	}
	return (1);
}

int	pipex(char **args, char **env, char *outfile, char *infile)
{
	t_cmd	*cmds;
	t_cmd	*head;
	int		pipe_fd[2];
	int		exit_status;

	// Always create outfile even if commands will fail
	create_empty_outfile(outfile);
	
	if (init_pipex(&cmds, args) == 1)
		return (1);
	head = cmds;

	/* Special case: only one command */
	if (!cmds->next)
	{
		exit_status = fork();
		if (exit_status < 0)
			return (free_cmd_int(head));
		if (exit_status == 0)
			one_cmd(cmds, env, infile, outfile);
		wait_all_children();
		free_cmd(head);
		return (0);
	}

	/* Handle first command - we continue even if it fails */
	if (handle_first_cmd(cmds, env, pipe_fd, infile) != 0)
	{
		free_cmd(head);
		return (1);
	}

	/* Process all middle commands - we continue even if they fail */
	if (process_middle_cmds(&cmds, env, pipe_fd) != 0)
	{
		free_cmd(head);
		return (1);
	}

	/* Handle the last command - we continue even if it fails */
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
	if (ac < 5 || !env || (ac < 6 && is_here_doc(av[1])))
	{
		if (ac < 5)
			write(2, "Error: Invalid number of arguments\n", 35);
		else if (!env)
			write(2, "Error: No environment\n", 22);
		else
			write(2, "Error: Not enough arguments for here_doc\n", 40);
		return (1);
	}
	else if (is_here_doc(av[1]) != 0)
	{
		save = here_doc(av + 1);
		if (save > 0)
		{
			pipex(av + 3, env, av[ac - 1], ".temp_here_doc.txt");
			unlink(".temp_here_doc.txt");
		}
		else if (save == -1)
		{
			write(2, "Error: here_doc failed\n", 23);
			// Create empty outfile anyway
			create_empty_outfile(av[ac - 1]);
		}
	}
	else
		pipex(av + 2, env, av[ac - 1], av[1]);
	return (0);
}