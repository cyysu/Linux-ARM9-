//segment of code which alters value of
//a variable in a child process. Identical
//to code which reads and changes register value.

//`addr' is address of variable being accessed.
data = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
printf("data = %d\n", data);
data = 245;
ptrace(PTRACE_POKEDATA, pid, addr, data);
ptrace(PTRACE_CONT, pid, 0, 0); 
wait(&status);

