/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 14:46:30 by sjo               #+#    #+#             */
/*   Updated: 2022/09/19 17:17:46 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

static void echoctl_off(void)
{
    struct termios attr;

    // STDIN_FILENO 터미널의 속성 받아오기
    tcgetattr(STDIN_FILENO, &attr);
    // ECHOCTL: 제어문자가 반향되도록 하는 플래그. 이 플래그를 off 시켜서 ctrl + c를 눌러도 ^C가 화면에 표시되지 않도록 한다.
    attr.c_lflag &= ~ECHOCTL;
    // 변경된 STDIN_FILENO 터미널 속성 적용
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

static void echoctl_on(void)
{
    struct termios attr;

    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

static void print_intro(void)
{
    int fd;
    char c;

    fd = open("intro.txt", O_RDONLY);
    while (read(fd, &c, 1) > 0)
        ft_putchar_fd(c, STDOUT_FILENO);
    ft_putendl_fd("", STDOUT_FILENO);
    close(fd);
}

int main(int argc, char **argv, char **envp)
{
    t_cmd_list *cmd_list;
    char *line;

    // 환경변수 저장
    g_env_list.env_list = copy_envs(envp);
    // 환경변수에 "OLDPWD" 없으면 get_pwd()로 받아온 현재 디렉토리 환경변수로 저장
    if (is_in_env_list("OLDPWD") == -1)
        edit_env_list(ft_strjoin(ft_strdup("OLDPWD="), get_pwd()));
    print_intro();
    while (1)
    {
        echoctl_off();
        set_main_signal();
        line = readline("minishell $ ");
        if (!line)
            break;
        if (*line != '\0')
            add_history(line);
        cmd_list = (t_cmd_list *)malloc(sizeof(t_cmd_list));
        if (!cmd_list)
            return (0);
        ft_memset(cmd_list, 0, sizeof(t_cmd_list));
        if (parse_cmd(line, &cmd_list) == 0)
        {
            free_cmd_list(cmd_list);
            continue;
        }
        echoctl_on();
    }
}