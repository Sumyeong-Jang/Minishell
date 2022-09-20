#include "../includes/minishell.h"

char *char_to_string(char c);
int is_white_space(char c);

char *char_to_string(char c)
{
	char *ret;

	ret = (char *)malloc(sizeof(char) * 2);
	if (ret == NULL)
		exit(1);
	ret[0] = c;
	ret[1] = '\0';
	return (ret);
}

int is_white_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (TRUE);
	return (FALSE);
}
