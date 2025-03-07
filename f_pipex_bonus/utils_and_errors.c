/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_and_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:15:22 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:25:07 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Return the length of a string
 * 
 * @param str String to measure
 * @return int Length of the string
 */

/**
 * @brief Return the length until a specific character
 * 
 * @param str String to measure
 * @param c Character to stop at
 * @return int Length until character c
 */
int	ft_strclen(char *str, int c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		i++;
	return (i);
}

/**
 * @brief Duplicate a portion of a string
 * 
 * @param str String to duplicate
 * @param n Number of characters to duplicate
 * @return char* New string, or NULL on failure
 */

/**
 * @brief Concatenate two strings
 * 
 * @param str First string
 * @param s Second string
 * @return char* New concatenated string, or NULL on failure
 */
char	*ft_strcat(char *str, char *s)
{
	int		i;
	int		j;
	char	*cat;

	cat = malloc(sizeof(char) * (ft_strclen(str, 0) + ft_strclen(s, 0) + 1));
	if (!cat)
		return (NULL);
	
	i = 0;
	j = 0;
	
	while (str && str[i])
	{
		cat[i] = str[i];
		i++;
	}
	
	while (s && s[j])
	{
		cat[i + j] = s[j];
		j++;
	}
	
	cat[i + j] = '\0';
	return (cat);
}

/**
 * @brief Print an error message
 * 
 * @param msg Error message
 */
void	print_error(char *msg)
{
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
}

/**
 * @brief Print an error message and return a value
 * 
 * @param msg Error message
 * @param ret_val Return value
 * @return int Return value
 */
int	error_return(char *msg, int ret_val)
{
	print_error(msg);
	print_error("\n");
	return (ret_val);
}

/**
 * @brief Report a malloc error
 * 
 * @return void* Always returns NULL
 */
void	*error_malloc(void)
{
	print_error("Error: malloc failed\n");
	return (NULL);
}