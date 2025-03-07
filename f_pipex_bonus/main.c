/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 17:30:18 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"


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

	if (init_pipex(&cmds, args) == 1)
		return (1);
	head = cmds;
	handle_first_cmd(cmds, env, pipe_fd, infile);
	process_middle_cmds(&cmds, env, pipe_fd);
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
	if (ac < 5 || env || (ac < 6 && is_here_doc(av[1])))
	{
		if (ac < 5)
			write(2, "Error: Invalid number of arguments\n", 35);
		else
			write(2, "Error: No environment\n", 23);
		return (1);
	}
	else if (is_here_doc(av[1]) != 0)
	{
		save = here_doc(av + 1);
		if (save > 0)
		{
			pipex(av + 2, env, av[ac - 1], ".temp_here_doc.txt");
			unlink(".temp_here_doc.txt");
		}
		else if (save == -1)
			write(2, "Error: here_doc failed\n", 23);
	}
	else
		pipex(av + 1, env, av[ac - 1], av[1]);
	return (0);
}
