/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:55:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:53:51 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Check if a string is a flag (starts with -)
 * 
 * @param str String to check
 * @return int 0 if it's a flag, 1 if not
 */
int	is_flag(char *str)
{
	if (str && str[0] == '-')
		return (0);
	return (1);
}

/**
 * @brief Special case for 'grep' command which accepts arguments differently
 * 
 * @param str Command string to check
 * @param k Counter to ensure we only identify grep once per command
 * @return int 0 if command is 'grep', 1 otherwise
 */
int	is_grep(char *str, int *k)
{
	if (*k == 0)
		*k = 1;
	else
		return (1);
	
	if (str && str[0] == 'g' && str[1] == 'r' && str[2] == 'e'
		&& str[3] == 'p')
		return (0);
	return (1);
}

/**
 * @brief Count the number of flags for a command
 * 
 * @param args Array of arguments
 * @param i Starting index
 * @return int Number of flags
 */
int	count_flags(char **args, int i)
{
	int	count;
	int	comp;

	comp = 0;
	while (args && args[comp])
		comp++;
	
	i++;
	count = 1;
	
	while (args && i <= comp && args[i])
	{
		if (is_flag(args[i]) == 0)
		{
			count++;
			i++;
		}
		else
			break;
	}
	
	return (count);
}

/**
 * @brief Parse a command and its arguments
 * 
 * @param args Array of arguments
 * @param i Pointer to current index (will be updated)
 * @param j Initial index for the result array
 * @param k Counter for grep detection
 * @return char** Array of command and arguments, or NULL on failure
 */
char	**parse_cmd_args(char **args, int *i, int j, int k)
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * (count_flags(args, *i) + 3));
	if (!cmd)
		return (NULL);
	
	// First argument is the command
	cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
	if (!cmd[j])
		return (free_str_array(cmd));
	
	j++;
	*i = *i + 1;
	
	// Parse flags and arguments
	while (args && args[*i])
	{
		if (is_flag(args[*i]) == 0 || is_grep(cmd[0], &k) == 0)
		{
			cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
			if (!cmd[j])
				return (free_str_array(cmd));
			
			*i = *i + 1;
			j++;
		}
		else
			break;
	}
	
	cmd[j] = 0;
	return (cmd);
}

/**
 * @brief Create a new command node
 * 
 * @param current Current command node
 * @param position Position in the pipeline
 * @return t_cmd* New command node, or NULL on failure
 */
t_cmd	*cmd_new(t_cmd *current, int position)
{
	t_cmd	*next;

	next = malloc(sizeof(t_cmd));
	if (!next)
		return (NULL);
	
	if (position == 0)
		current->prev = NULL;
	
	current->next = next;
	current->position = position;
	next->prev = current;
	next->args = NULL;
	next->next = NULL;
	
	return (next);
}

/**
 * @brief Parse all commands from arguments
 * 
 * @param args Array of arguments
 * @return t_cmd* Linked list of commands, or NULL on failure
 */
t_cmd	*parse_commands(char **args)
{
	t_cmd	*cmd;
	t_cmd	*save;
	int		i;
	int		position;

	if (!args || !args[0])
		return (NULL);
	
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	
	save = cmd;
	i = 0;
	position = 0;
	
	while (args && args[i] && args[i + 1])
	{
		cmd->args = parse_cmd_args(args, &i, 0, 0);
		if (!cmd->args)
			return (free_cmd_list(save));
		
		cmd = cmd_new(cmd, position);
		if (!cmd)
			return (free_cmd_list(save));
		
		position++;
	}
	
	cmd->next = NULL;
	return (save);
}
