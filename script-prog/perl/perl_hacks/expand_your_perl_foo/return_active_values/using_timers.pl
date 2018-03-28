my $task_timer    = timer();
my $subtask_timer = timer();

for my $task (@tasks)
{
    print "Performing $task...\n";
    for my $subtask ($task->get_subtasks())
    {
        $subtask->perform();
        print "\t$subtask took $subtask_timer seconds\n";
    }
    print "Finished $task in $task_timer seconds\n\n";
}
