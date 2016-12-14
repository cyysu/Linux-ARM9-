//compile into `child2'
main()
{
    /* Note that we are not calling ptrace(PTRACE_TRACEME,...)
     * here. That will be done by the process which has
     * exec'd this one.
     */
    printf("child starts...\n");
    sleep(1);
    while(1) printf("hello\n");
}
