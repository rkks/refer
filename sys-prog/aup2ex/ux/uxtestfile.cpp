/*
	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "ux.hpp"

using namespace Ux;

static void terminal_test(void)
{
	Terminal trm(STDIN_FILENO);
	char buf[1000];

	cout << endl << "terminal test" << endl;
	cout << "isatty: " << trm.isatty() << endl;
	trm.ttyname(buf, sizeof(buf));
	cout << "ttyname: " << buf << endl;
	Termios tios;
	trm.tcgetattr(tios);
	cout << "i speed: " << tios.cfgetispeed() << endl;
	cout << "o speed: " << tios.cfgetospeed() << endl;
}

static void posixshm_test(void)
{
	const char *shm_path = "/posix_shm";
	PosixShm m;

	cout << endl << "POSIX shared memory test" << endl;
	
	try {
		m.unlink(shm_path);
	}
	catch (Error& e) {
		if (e == ENOSYS) {
			cout << "*** POSIX shared memory not supported." << endl;
			return;
		}
		// otherwise ignore the error
	}
	cout << "Getting ready to open..." << endl;
	EC_CATCH( m.open(shm_path, O_RDWR | O_CREAT) )
	cout << "... opened" << endl;
	m.truncate(10000);
	char *p = (char *)m.mmap(10000);
	strcpy(p, "This is data for POSIX shared memory.");
	m.munmap(p, 10000);
	p = (char *)m.mmap(10000);
	cout << "Got: \"" << p << "\"" << endl;	
	m.unlink(shm_path);
}

static void posixmsg_test(void)
{
	const char *msg_path = "/posix_mq2";
	PosixMsg m;
	const char *s = "A POSIX message.";
	char buf[2000];
	ssize_t n;
	struct mq_attr attr;

	cout << endl << "POSIX message test" << endl;
	
	try {
		m.unlink(msg_path);
	}
	catch (Error& e) {
		if (e == ENOSYS) {
			cout << "*** POSIX messages not supported." << endl;
			return;
		}
		// otherwise ignore the error
	}
	cout << "Getting ready to open..." << endl;
	EC_CATCH( m.open(msg_path, O_RDWR | O_CREAT) )
	cout << "... opened" << endl;
	EC_CATCH( m.getattr(&attr) )
	cout << "maxmsg: " << attr.mq_maxmsg << "; msgsize: " << attr.mq_msgsize << endl;
	EC_CATCH( m.send(s, strlen(s)) )
	EC_CATCH( n = m.receive(buf, sizeof(buf) - 1) )
	buf[n] = '\0';
	cout << "Got: \"" << buf << "\"" << endl;
	EC_CATCH( m.close() )
	EC_CATCH( m.unlink(msg_path) )
	cout << "POSIX message text finished" << endl;
}

static void handler(int signum)
{
	cout << "Got signal " << signum << endl;
}

static void sysvsem_test(void)
{
	const char *sem_path = "sysv_sem";
	union semun arg;
	struct sembuf sbuf;

	cout << endl << "SysV semaphore test" << endl;
	File f(sem_path);
	EC_CATCH( f.open(O_WRONLY | O_CREAT) )
	f.close();
	key_t k = SysVIPC::ftok(sem_path, 1);
	SysVSem s;
	EC_CATCH( s.get(k, 1) )
	arg.val = 0;
	EC_CATCH( s.ctl(0, SETVAL, arg) )
	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;
	EC_CATCH( s.op(&sbuf, 1) )
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	Process::sigaction(SIGALRM, &act);
	Timer::alarm(2);
	sbuf.sem_op = 0;
	try {
		s.op(&sbuf, 1);
	}
	catch (const Error& e) {
		if (e == EINTR)
			cout << "op interrupted, as expected" << endl;
		else
			throw;
	}
	EC_CATCH( s.ctl(0, IPC_RMID, arg) )
	cout << "SysV semaphore test finished" << endl;
}

static void posixsem_test(void)
{
	const char *sem_path = "/posix_sem";

	cout << endl << "Posix semaphore test" << endl;
	PosixSem s;
	try {
		s.open(sem_path, O_RDWR | O_CREAT);
	}
	catch (Error& e) {
		if (e == ENOSYS) {
			cout << "*** POSIX semaphores not supported." << endl;
			return;
		}
		else
			EC_EXIT(e)
	}
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	Process::sigaction(SIGALRM, &act);
	Timer::alarm(2);
	try {
		s.wait();
	}
	catch (const Error& e) {
		if (e == EINTR)
			cout << "wait interrupted, as expected" << endl;
		else
			throw;
	}
	EC_CATCH( s.post() )
	EC_CATCH( s.wait() ) // should not block
	EC_CATCH( s.close() )
	EC_CATCH( s.unlink(sem_path) )
	cout << "POSIX semaphore test finished" << endl;
}

static void sysvshm_test(void)
{
	const char *shm_path = "sysv_shm";

	cout << endl << "SysV shared memory test" << endl;
	File f(shm_path);
	EC_CATCH( f.open(O_WRONLY | O_CREAT) )
	f.close();
	key_t k = SysVIPC::ftok(shm_path, 1);
	SysVShm m;
	EC_CATCH( m.get(k, 1000) )
	char *p = (char *)m.at();
	strcpy(p, "Some data for the shared memory segment.");
	m.dt(p);
	p = (char *)m.at();
	cout << "Got: \"" << p << "\"" << endl;	
	m.ctl(IPC_RMID);
}

static void sysvmsg_test(void)
{
	const char *msgq_path = "sysv_msgq";
	struct {
		long mtype;
		char mtext[1000];
	} msg = { 1 }, msg2;

	cout << endl << "SysV message test" << endl;
	File f(msgq_path);
	EC_CATCH( f.open(O_WRONLY | O_CREAT) )
	f.close();
	key_t k = SysVIPC::ftok(msgq_path, 1);
	cout << "key: " << k << endl;

	SysVMsg q;
	EC_CATCH( q.get(k) )
	strcpy(msg.mtext, "SysV message");
	EC_CATCH( q.snd(&msg, strlen(msg.mtext)) )
	ssize_t n;
	EC_CATCH( n = q.rcv(&msg2, sizeof(msg2.mtext) - 1) )
	msg2.mtext[n] = '\0';
	cout << "Got: \"" << msg2.mtext << "\"" << endl;
	q.ctl(IPC_RMID);
}

static void socket_test_unix(void)
{
	Socket s("MySocket");
	Process p;
	File pfd[2];
	
	try {
		s.unlink();
	}
	catch (...) {
	};
	SockAddrUn sa(s);
	File::pipe(pfd);
	if ((p = Process::fork()) == 0) {
		char c;

		pfd[1].close();
		pfd[0].read(&c, 1);
		pfd[0].close();
		EC_CATCH( s.socket() )
		EC_CATCH( s.connect(sa) )
		cout << "Child connected!" << endl;
		EC_CATCH( s.write("Hello", 6) )
		s.close();
		Process::exit();
	}
	else {
		Socket client;
		char msg[100];
	
		EC_CATCH( s.socket() )
		EC_CATCH( s.bind(sa) )
		EC_CATCH( s.listen() )
		pfd[0].close();
		pfd[1].close();
		EC_CATCH( client = s.accept() )
		cout << "Parent accepted!" << endl;
		EC_CATCH( client.read(msg, sizeof(msg)) )
		client.close();
		s.close();
		cout << "Parent got message: " << msg << endl;
	}
}

static void socket_test_inet(void)
{
	Socket s;
	Process p;
	File pfd[2];

cout << 1 << endl;
#if defined(SOLARIS)
	SockAddrIn sa(1001, "192.168.0.10"); // Sol
#elif defined(LINUX)
	SockAddrIn sa(1001, "192.168.0.19"); // Suse
#endif
cout << 2 << endl;
	File::pipe(pfd);
	if ((p = Process::fork()) == 0) {
		char c;

		pfd[1].close();
		pfd[0].read(&c, 1);
		pfd[0].close();
		EC_CATCH( s.socket(AF_INET) )
		EC_CATCH( s.connect(sa) )
		cout << "Child connected!" << endl;
		EC_CATCH( s.write("Hello", 6) )
		s.close();
		Process::exit();
	}
	else {
		Socket client;
		char msg[100];
	
		EC_CATCH( s.socket(AF_INET) )
		EC_CATCH( s.bind(sa) )
		EC_CATCH( s.listen() )
		pfd[0].close();
		pfd[1].close();
		EC_CATCH( client = s.accept() )
		cout << "Parent accepted!" << endl;
		EC_CATCH( client.read(msg, sizeof(msg)) )
		client.close();
		s.close();
		cout << "Parent got message: " << msg << endl;
	}
}

static void sockip_test(void)
{
	char buf[100];

	SockIPv4 si4("64.81.102.0");
	cout << "As IPv4: " << si4.get_ipv4() << endl;
	cout << "As dotted: " << si4.get_string(buf, sizeof(buf)) << endl;
	SockIPv6 si6("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210");
	cout << "As IPv6:";
	for (int i = 0; i < 16; i++)
		cout << " " << (int)si6.get_ipv6()[i];
	cout << endl;
	cout << "As dotted: " << si6.get_string(buf, sizeof(buf)) << endl;
}

static void browser_test(void)
{
	SockAddr sa;
	Socket s;
	char buf[512];
	const char *request = "GET / HTTP/1.0\n\n";

	sa.set_server("www.basepath.com");
	s.socket(AF_INET, SOCK_STREAM, sa.get_protocol());
	s.connect(sa);
	s.write(request, strlen(request));
	ssize_t n = s.read(buf, sizeof(buf) - 1);
	buf[n] = '\0';
	cout << endl << endl << buf << endl<< endl;
}

static void new_time_test(void)
{
	TimeParts tm;
	TimeString ts;
	TimeSec tnow(now);
	TimeString snow(tnow);
	TimeString snow2((time_t)TimeSec());
	TimeString snow3((time_t)TimeSec(now));
	TimeMsec tnowmsec(now), tepochmsec(epoch);
	TimeNsec tnownsec(now), tepochnsec(epoch);

	cout << endl << "new_time_test" << endl;
	cout << "now: " << snow;
	cout << "now: " << snow3;
	cout << "epoch (TimeString): " << snow2;
	cout << "epoch (TimeParts): " << tm << endl;;
	tm.set_fmt("12-June-2003", "%d-%b-%Y");
	cout << "My Birthday (wrong wday): " << tm << endl;
	cout << "My Birthday (right wday): " << TimeParts(tm.get_secs()) << endl;
	char sbuf[100];
	tm.get_fmt(sbuf, sizeof(sbuf), "%x %X");
	cout << "My Birthday (right wday, strftime): " << sbuf << endl;
	try {
		tm.set("12-June-2003");
		cout << "My Birthday: " << tm << endl;
	}
	catch (const Error& e) {
		cout << "set(const char *) failed, as expected: " << e << endl;
	}
	cout << "ts as constructed: " << ts << endl;
	ts.set(INT_MIN);
	cout << "bgn of time: " << ts;
	ts.set(INT_MAX);
	cout << "end of time: " << ts;
	cout << "now in msecs: " << tnowmsec.get_string(sbuf, sizeof(sbuf)) << endl;
	cout << "now in msecs: " << tnowmsec << endl;
	cout << "epoch in msecs: " << tepochmsec << endl;
	cout << "now in nsecs: " << tnownsec.get_string(sbuf, sizeof(sbuf)) << endl;
	cout << "now in nsecs: " << tnownsec << endl;
	cout << "epoch in nsecs: " << tepochnsec << endl;

#if 0
	cout << "Epoch: " << tm << endl;
	cout << "Now: " << Timestr().ctime();
	tm.tm_year = 98;
	cout << "1998: " << tm << endl;
#endif
}

static void time_test(void)
{
	Timetm tm;
	Timestr ts;

	cout << "Epoch: " << tm << endl;
	cout << "Now: " << Timestr().ctime();
	tm.tm_year = 98;
	cout << "1998: " << tm << endl;
	cout << "ts as constructed: " << ts << endl;
	cout << "bgn of time: " << Timestr().ctime(INT_MIN);
	cout << "end of time: " << Timestr().ctime(INT_MAX);
	tm.strptime("12-June-2003", "%d-%b-%Y");
	cout << "My Birthday (wrong wday): " << tm << endl;
	cout << "My Birthday (right wday): " << Timetm(tm.mktime()) << endl;
	try {
		tm.getdate("12-June-2003");
		cout << "My Birthday: " << tm << endl;
	}
	catch (const Error& e) {
		cout << "getdate failed, as expected: " << e << endl;
	}
	char sbuf[100];
	tm.strftime(sbuf, sizeof(sbuf), "%x %X");
	cout << "My Birthday (right wday, strftime): " << sbuf << endl;
}

static void sigset_test(void)
{
	Sigset set;

	set.add(SIGINT);
	if (!set.ismember(SIGINT))
		cout << "add error" << endl;
	set.del(SIGINT);
	if (set.ismember(SIGINT))
		cout << "del error" << endl;
	set.fill();
	if (!set.ismember(SIGINT) || !set.ismember(SIGUSR1))
		cout << "fill error" << endl;
	cout << "+++ End of sigset_test +++" << endl;
}

static void at_exit_fcn(void)
{
	cout << endl << "atexit fcn called" << endl;
}

static void dirstream_test(void)
{
	DirStream d;
	int i;
	long loc;

	d.open_alloc("/aup");
	for (i = 0; d.read(); i++) {
		cout << "dir entry " << i << ": " << d.get_name() << endl;
		if (i == 3)
			loc = d.tell();
	}
	d.seek(loc);
	if (d.read())
		cout << "dir entry 4 again: " << d.get_name() << endl;
	d.rewind();
	if (d.read())
		cout << "dir entry 1 again: " << d.get_name() << endl;
	d.close();
	cout << "+++ End of dirstream_test +++" << endl;
}

static void process_test(void)
{
	Process p;
	ExitStatus status;
	
	Process::atexit(at_exit_fcn);
	if ((p = Process::fork()) == 0) {
		try {
			Process::execlp("echo", "echo", "arg_one", "arg_two", "arg_three", NULL);
		}
		catch (const Error& e) {
			EC__EXIT(e)
		}
	}
	p.waitpid(&status);
	cout << "Process " << p << " exit status: " << status << endl;
	if ((p = Process::fork()) == 0) {
		char *av[] = {"sh", "-c", "set", NULL };
		char *ev[] = {"SOMETHING=DOGCOW", NULL};

		try {
			Process::execvpe("sh", av, ev);
		}
		catch (const Error& e) {
			EC__EXIT(e)
		}
	}
	p.waitpid(&status);
	cout << "Process " << p << " exit status: " << status << endl;
	if ((p = Process::fork()) == 0) {
		try {
			Process::pause();
		}
		catch (const Error& e) {
			EC__EXIT(e)
		}
	}
	Clock::sleep(1);
	Clock::usleep(123456);
	p.kill(SIGFPE);
	p.waitpid(&status);
	cout << "Process " << p << " exit status: " << status << endl;

	Dir dcwd;
	dcwd.alloc();
	Process::getcwd(dcwd);
	cout << dcwd << endl;
	
	Dir d(".");
	d.open(O_RDONLY);
	Process::chdir("/tmp");
	Process::getcwd(dcwd);
	cout << dcwd << endl;
	Process::chdir(d.get_fd());
	d.close();
	Process::getcwd(dcwd);
	cout << dcwd << endl;
	dcwd.free();

	struct rlimit rl;
	Process::getrlimit(RLIMIT_DATA, &rl);
	cout << "RLIMIT_DATA: cur = " << rl.rlim_cur << "; max = " << rl.rlim_max << endl;
	struct rusage usage;
	Process::getrusage(RUSAGE_SELF, &usage);
	cout << "Time: " << usage.ru_utime << endl;
	Process::nice(10);
	rl.rlim_cur = 0;
	Process::setrlimit(RLIMIT_CORE, &rl);
	try {
		Process::chroot("/tmp");
	}
	catch (const Error& e) {
		cout << e << endl;
	}
	cout << "+++ End of process_test +++" << endl;
}

static void process_env_test(void)
{
	try {
		cout << "HOME = " << getenv("HOME") << endl;
		Process::putenv("NEWVAR=newvalue");
		cout << "NEWVAR = " << getenv("NEWVAR") << endl;
		Process::unsetenv("NEWVAR");
		cout << "NEWVAR = " << getenv("NEWVAR") << endl;
		Process::setenv("NEWVAR", "another value", true);
		cout << "NEWVAR = " << getenv("NEWVAR") << endl;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
/*
	catch (const Error& e) {
		cout << "process_env_test caught an error: " << e << endl;
	}
*/
	cout << "+++ End of process_env_test +++" << endl;
}

static void pipe_test(void)
{
	File pf[2];
	char buf[100];
	
	File::pipe(pf);
	pf[1].write("Something", 10);
	pf[0].read(buf, sizeof(buf));
	File(STDOUT_FILENO).write(buf, strlen(buf));
	File(STDOUT_FILENO).write("\n", 1);
	cout << "+++ End of pipe_test +++" << endl;
}

#define PATH "/aup/aup2ex.tar"
#define FREQ 8000

static void aio_test(void)
{
	File f(PATH);
	int count = 0, e;
	char buf1[512], buf2[512];
	Aio cb;
	const struct aiocb *list[1] = { &cb };

	cb.aio_fildes = STDIN_FILENO;
	cb.aio_buf = buf2;
	cb.aio_nbytes = sizeof(buf2);
	cb.aio_sigevent.sigev_notify = SIGEV_NONE;
	f.open(O_RDONLY);
	timestart();
	while (f.read(buf1, sizeof(buf1)) > 0) {
		if (count % FREQ == 0) {
			if (count > 1) {
				Aio::suspend(list, 1);
				if ((e = cb.error()) != 0)
					throw Error(e);
			}
			cb.read();
		}
		count++;
	}
	timestop("asynchronous");
	printf("read %d blocks\n", count);
}

int main(void)
{
	File file("tmp"), file2;
	ssize_t n;
	char buf[100];
	struct stat sbuf;

	try {
		sigset_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		process_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	process_env_test();
	try {
		aio_test();
	}
	catch (const Error& e) {
		if (e == ENOSYS)
			cout << e << endl;
		else
			EC_EXIT(e)
	}
	try {
		file.open(O_RDWR | O_CREAT | O_TRUNC);
		if (file.write("Hello", 5) != 5)
			throw Error(EIO);
		file2 = file.dup();
		if (file2.seek(0, SEEK_SET) != 0)
			throw Error(EIO);
		n = file2.read(buf, sizeof(buf) - 1);
		buf[n] = '\n';
		buf[n + 1] = '\0';
		File(STDOUT_FILENO).write(buf, n + 1);
		file2.stat(&sbuf);
		cout << "Size: " << sbuf.st_size << endl;
		file2.close();
		file2 = file.dup2(10);
		file2.stat(&sbuf);
		cout << "Size: " << sbuf.st_size << endl;
		file.stat(&sbuf);
		cout << "Size: " << sbuf.st_size << endl;
		File("./tmp").stat(&sbuf);
		cout << "Size: " << sbuf.st_size << endl;
		pipe_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		File myfile_in("myfifo");
		File myfile_out = myfile_in;
		try { myfile_in.unlink(); } catch (...) { };
		myfile_in.mkfifo();
		myfile_in.open(O_RDONLY | O_NONBLOCK);
		myfile_out.open(O_WRONLY);
		myfile_out.fcntl(F_SETFL, myfile_out.fcntl(F_GETFL) & ~O_NONBLOCK);
		char *s = "Stuff to FIFO";
		myfile_out.write(s, strlen(s) + 1);
		myfile_in.read(buf, sizeof(buf));
		cout << "Read from FIFO: " << buf << endl;
		
		Dir d("barfdir");
		cout << d << endl;
		EC_CATCH( d.mkdir() )
		cout << "dir access = " << d.access(F_OK, false) << endl;
		EC_CATCH( d.rmdir() )
		EC_CATCH( cout << "dir access = " << d.access(F_OK, false) << endl )
		//maybe bool with 3 errnos collected into false?
		//symlink subclass of file
	}
	catch (const Error& e) {
		EC_CAUGHT(e)
		exit(EXIT_FAILURE);
	}
	try {
		//dirstream_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		size_t n;
		char buf[200];

		n = System::confstr(_CS_PATH, buf, sizeof(buf));
		cout << "_CS_PATH: " << buf << " (" << n << ")" << endl;
		cout << "_SC_ARG_MAX: " << System::sysconf(_SC_ARG_MAX) << endl;
		struct utsname name;
		System::uname(&name);
		cout << "sysname: " << name.sysname << endl;
		cout << "nodename: " << name.nodename << endl;
		cout << "release: " << name.release << endl;
		cout << "version: " << name.version << endl;
		cout << "machine: " << name.machine << endl;
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		time_test();
	}
	catch (const Error& e) {
		cout << e << endl;
		EC_EXIT(e)
	}
	try {
		new_time_test();
	}
	catch (const Error& e) {
		cout << e << endl;
		EC_EXIT(e)
	}
	try {
		socket_test_unix();
		socket_test_inet();
		sockip_test();
		browser_test();
	}
	catch (const Error& e) {
		cout << e << endl;
		EC_EXIT(e)
	}
	try {
		sysvmsg_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		sysvshm_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		sysvsem_test();
	}
	catch (const Error& e) {
		EC_EXIT(e)
	}
	try {
		posixmsg_test();
	}
	catch (const Error& e) {
		cout << e << endl;
	}
	try {
		posixsem_test();
	}
	catch (const Error& e) {
		cout << e << endl;
	}
	try {
		posixshm_test();
	}
	catch (const Error& e) {
		cout << e << endl;
	}
	try {
		terminal_test();
	}
	catch (const Error& e) {
		cout << e << endl;
		EC_EXIT(e)
	}

	exit(EXIT_SUCCESS);
}
