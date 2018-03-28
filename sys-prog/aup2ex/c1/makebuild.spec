!include common/makebuild.spec

ckstdvers c1/ckstdvers.c
ckvers -DCKVERS_UNIX98 c1/ckvers.c
err c1/errorhandling.c
opttest c1/opttest.c
suvreq -DCKVERS_UNIX98x c1/suvreq.c
sysconf c1/sysconf.c
timetst c1/timetst.c
version c1/version.c
