/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:42:43 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/10 16:25:56 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_data(t_data *data, char **args, char **env, int ac)
{
	t_cmd	*cmd;
	int     cmd_start_idx;
	int     cmd_end_idx;

	data->env = env;
	data->here_doc = is_here_doc(args[1]);
	data->cmd_count = 0;
	data->cmd_list = NULL;
	data->fd.in_file = -1;
	data->fd.out_file = -1;
	data->fd.pipes = NULL;
	data->fd.pipe_count = 0;
	if (data->here_doc)
		cmd_start_idx = 3;
	else
		cmd_start_idx = 2;
	cmd_end_idx = ac - 1; // Last arg is outfile
	data->cmd_list = parse_commands(args + cmd_start_idx, cmd_end_idx - cmd_start_idx);
	if (!data->cmd_list)
		return (error_return("Error: Failed to parse commands", 1));
	cmd = data->cmd_list;
	while (cmd)
	{
		data->cmd_count++;
		cmd = cmd->next;
	}
	if (setup_files(data, args, ac) != 0)
		return (clean_exit(data, 1, NULL));
	if (init_pipes(data, ac, args) != 0)
		return (clean_exit(data, 1, NULL));
	return (0);
}

int	pipex(char **args, char **env, int ac)
{
	t_data	data;

	if (init_data(&data, args, env, ac) != 0)
		return (1);
	if (run_pipeline(&data) != 0)
		return (clean_exit(&data, 1, NULL));
	return (clean_exit(&data, 0, NULL));
}

int	main(int ac, char **av, char **env)
{
	int	here_doc_status;
	int	exit_status;

	if (ac < 5 || (ac < 6 && is_here_doc(av[1])))
		return (error_return("Error: Invalid number of arguments", 1));
	if (is_here_doc(av[1]))
	{
		here_doc_status = setup_here_doc(av + 2);
		if (here_doc_status == -1)
			return (1);
		exit_status = pipex(av, env, ac);
		unlink(".temp_here_doc.txt");
		return (exit_status);
	}
	else
	{
		exit_status = pipex(av, env, ac);
		return (exit_status);
	}
}
