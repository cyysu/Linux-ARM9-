//wait for child to stop at the `exec'
wait(&status);
while(1) {
     // do just one instruction
     ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
     wait(&status);
     if(WIFEXITED(status)) break;
     // `addr' is address of `i' in child process
     dat = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
     if(dat == 10) {
           ptrace(PTRACE_POKEDATA, pid, addr, 2341);
           ptrace(PTRACE_CONT, pid, 0, 0);
           break;
      }
}
wait(&status);
if(WIFEXITED(status))printf("child over\n");

