#include "../includes/minishell.h"

int	is_right_form(char *str);

int	is_right_form(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[0]) == FALSE && str[0] != '_')
		return (FALSE);
	if (has_equal_sign(str) == TRUE)
	{
		while (str[i] != '=')
		{
			if (ft_isalnum(str[i]) == FALSE && str[i] != '_')
				return (FALSE);
			i++;
		}
		return (TRUE);
	}
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == FALSE && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}