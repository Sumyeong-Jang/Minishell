/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 19:06:28 by sjo               #+#    #+#             */
/*   Updated: 2022/09/21 03:35:25 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void sig_heredoc_handler(int sig)
{
    (void)sig;
    ft_putendl_fd("", STDOUT_FILENO);
    exit(130);
}

void set_heredoc_signal(void)
{
    signal(SIGINT, sig_heredoc_handler);
    signal(SIGQUIT, SIG_IGN);
}

void sigint_handler(int sig)
{
    // ctrl + c
    pid_t pid;

    // waitpid: 프로세스의 종료 상태를 리턴해 주는 함수: 성공시 프로세스 ID, 오류시 -1, 그 외에는 0 리턴
    pid = waitpid(-1, NULL, WNOHANG);
    if (sig == SIGINT)
    {
        // 자식 프로세스 없으면
        if (pid == -1)
        {
            ft_putendl_fd("", STDOUT_FILENO);
            // line buffer flush(readline 실행 중 들어온 입력을 첫번째 인자로 다시 설정)
            rl_replace_line("", 0);
            // 커서가 개행 문자를 통해 다음 줄로 이동했음을 알려주는 함수(rl_redisplay를 실행하기 위해 필요)
            rl_on_new_line();
            // 화면에 출력된 값을 rl_line_buffer 안에 저장된 내용물로 바꾼다.(readline실행 중 다시 readline을 실행할 때)
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