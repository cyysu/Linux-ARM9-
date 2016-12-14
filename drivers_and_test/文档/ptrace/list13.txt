#include <sys/ptrace.h>
#include <signal.h>
#include <linux/user.h>
#include <sys/types.h>
#include <sys/wait.h>
main()
{
    
    int  status = 0, pid, r;
    struct user_regs_struct uregs;
    if ((pid=fork())==0) {
        printf("pid = %d, ppid = %d\n", getpid(), getppid());
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        kill(getpid(), SIGINT);
        r = getpid();
        printf("%d\n", r);
    } else {
        wait(&status);
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        wait(&status);
        ptrace(PTRACE_GETREGS, pid, 0, &uregs);
        //this should print 20, syscall number of getpid
        printf("syscall nr: %d\n", uregs.orig_eax);
        // 64 is syscall number of getppid
        uregs.orig_eax = 64;
        ptrace(PTRACE_SETREGS, pid, 0, &uregs);
        ptrace(PTRACE_CONT, pid, 0, 0);
        wait(&status);
        if(WIFEXITED(status))printf("child over\n");
        sleep(1000);
    }
}

