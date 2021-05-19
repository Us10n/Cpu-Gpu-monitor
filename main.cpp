#include "Controller.h"
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <signal.h>

bool flags[2] = {false, false}; //flags[0]-gpu enable, flags[1]-cpu enable
int rmode = -1;                  //refreshing mode
bool endprog = false;
bool forceend=false;
void my_hndl(int);
void *child(void *);
bool getFlagsAndDelay(int, char **);
bool isNumber(std::string);
bool isCorrectSize();

int main(int argc, char *argv[])
{
    if (!isCorrectSize())
    {
        std::cout << "Change terminal resolution al least to 80x24\n";
        return 0;
    }
    system("stty -echo");
    printf("%c[?25l", 0x1B);
    printf("%c[=2h", 0x1B);
    Controller controller;

    if (!getFlagsAndDelay(argc, argv))
    {
        system("stty echo");
        printf("%c[?25h", 0x1B);
        return 0;
    }
    pthread_t thread;
    int status = pthread_create(&thread, NULL, child, NULL);
    if (status != 0)
    {
        fprintf(stdout, "Thread_create error occured");
        exit(errno);
    }
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
    {
        fprintf(stdout, "SIGINT block error");
        exit(errno);
    }
    bool isRunning;
    do
    {
        isRunning = controller.Start(flags, rmode);
        sleep(rmode);
        if(!isCorrectSize()){
            forceend=true;
            break;
        }
    } while (!endprog && isRunning);
    endprog = true;
    if (pthread_join(thread, NULL) != 0)
    {
        fprintf(stdout, "Child wait error\n");
        exit(errno);
    }
    system("stty echo");
    printf("%c[?25h\n", 0x1B);
    if(forceend){
        system("clear");
        std::cout<<"Terminal size was criticaly changed\n";
    }
    return 0;
}

bool getFlagsAndDelay(int argc, char **argv)
{
    bool isNextTime = false;
    if (argc > 5)
    {
        std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
        return false;
    }
    ++argv;
    while (*argv)
    {
        if (argv[0][0] != '-')
        {
            std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
            return false;
        }

        switch (argv[0][1])
        {
        case 'h':
        {
            if (argv[0][2])
            {
                std::cout << "Unknown argument" << *argv << ". Run './monitor -h' for help.\n";
                return false;
            }
            std::cout << "CPU/GPU Monitoring v1.3\nStart program with terminal resolution more than 80x24 only!\n"
                      << "[OPTIONS]:\n"
                      << std::setw(7) << std::right << "-h:"
                      << "       Print usage information and exit\n"
                      << std::setw(7) << std::right << "-g:"
                      << "       Display information about GPU only\n"
                      << std::setw(7) << std::right << "-c:"
                      << "       Display information about CPU only\n"
                      << std::setw(7) << std::right << "-s:"
                      << "       Refresh speed(3 modes from 0 to 2)\n";
            return false;
        }
        break;
        case 's':
        {
            if (argv[0][2])
            {
                std::cout << "Unknown parameter for the argument -s" << *argv << ". Run './monitor -h' for help.\n";
                return false;
            }
            ++argv;
            if (!*argv || argv[0][0] == '-' || rmode>0)
            {
                std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
                return false;
            }
            else
            {
                std::string time = *argv;
                if (time.size() > 2 || !isNumber(time))
                {
                    std::cout << "Unknown parameter for the argument -s" << *argv << ". Run './monitor -h' for help.\n";
                    return false;
                }
                rmode = std::stoi(time);
                if (rmode < 0 || rmode > 2)
                {
                    std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
                    return false;
                }
            }
        }
        break;
        case 'g':
        {
            if (flags[0] == true)
            {
                std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
                return false;
            }
            flags[0] = true;
            if (argv[0][2])
            {
                if (argv[0][2] == 'c' && !argv[0][3])
                {
                    flags[1] = true;
                }
                else
                {
                    std::cout << "Unknown argument " << *argv << ". Run './monitor -h' for help.\n";
                    return false;
                }
            }
        }
        break;
        case 'c':
        {
            if (flags[1] == true)
            {
                std::cout << "Invalid combination of input arguments. Run './monitor -h' for help.\n";
                return false;
            }
            flags[1] = true;
            if (argv[0][2])
            {
                if (argv[0][2] == 'g' && !argv[0][3])
                {
                    flags[0] = true;
                }
                else
                {
                    std::cout << "Unknown argument " << *argv << ". Run './monitor -h' for help.\n";
                    return false;
                }
            }
        }
        break;

        default:
        {
            std::cout << "Unknown argument " << *argv << ". Run './monitor -h' for help.\n";
            return false;
        }
        break;
        }
        ++argv;
    }
    if (!flags[0] && !flags[1])
    {
        flags[0] = flags[1] = true;
    }
    if(rmode==-1){
        rmode=0;
    }
    return true;
}
bool isNumber(std::string str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}
bool isCorrectSize()
{
    FILE *in;
    char buff[512];

    if (!(in = popen("stty size", "r")))
    {
        return false;
    }
    fgets(buff, sizeof(buff), in);
    std::string resolution = buff;
    int height = std::stoi(resolution.substr(0, resolution.find_first_of(' ')));
    int width = std::stoi(resolution.substr(resolution.find_first_of(' ')+1, 3));
    if (height < 24 || width < 80)
    {
        return false;
    }
    return true;
}

void my_hndl(int sig)
{
    if (sig == SIGINT)
    {
        endprog = true;
    }
}
void *child(void *smth)
{
    struct sigaction action;
    //sigaction "config" on SIGINT signal
    action.sa_handler = my_hndl;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, 0) == -1)
    {
        fprintf(stdout, "Signaction error");
        exit(errno);
    }

    while (!endprog)
    {
    };

    pthread_exit(0);
}