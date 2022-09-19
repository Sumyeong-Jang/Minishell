/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 19:06:28 by sjo               #+#    #+#             */
/*   Updated: 2022/09/19 15:38:17 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void sigint_handler(int sig)
{
    pid_t pid;

    // waitpid: 프로세스의 종료 상태를 리턴해 주는 함수: 성공시 프로세스 ID, 오류시 -1, 그 외에는 0 리턴
    pid = waitpid(-1, NULL, WNOHANG);
    if (sig == SIGINT)
    {
        if (pid == -1)
        {
            ft_putendl_fd("", STDOUT_FILENO);
            // line buffer flush
            rl_replace_line("", 0);
            // 커서가 개행 문자를 통해 다음 줄로 이동했음을 알려주는 함수(rl_redisplay를 실행하기 위해 필요)
            rl_on_new_line();
            // readline 함수의 인자로 넣은 문자열을 다시 출력한다.(rl_replace_line를 출력하지 않으면 작동하지 않는다.)
            rl_redisplay();
            g_env_list.exit_status = 1;
        }
        else
        {
            g_env_list.exit_status = 130;
            ft_putendl_fd("", STDOUT_FILENO);
        }
    }
}

void sigquit_handler(int sig)
{
    pid_t pid;

    pid = waitpid(-1, NULL, WNOHANG);
    if (sig == SIGQUIT)
    {
        if (pid == -1)
        {
            rl_on_new_line();
            rl_redisplay();
        }
        else
        {
            g_env_list.exit_status = 131;
            ft_putstr_fd("Quit: ", STDOUT_FILENO);
            ft_putnbr_fd(sig, STDOUT_FILENO);
            ft_putendl_fd("", STDOUT_FILENO);
        }
    }
}

void set_main_signal(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
}