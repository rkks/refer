#include <stdio.h>
#include <sys/vfstab.h>
#include "ssp.h"

static void print_vfstab (struct vfstab *vp);

int main (int argc, char **argv)
{
	FILE *fp;
	struct vfstab vp;
	int ret;
	int i;

	if ((fp = fopen ("/etc/vfstab", "r")) == NULL)
		err_msg ("Can't open /etc/vfstab");

	if (argc == 1) {
		while ((ret = getvfsent (fp, &vp)) == 0)
			print_vfstab (&vp);

		if (ret != -1)
			err_quit ("Bad /etc/vfstab file.\n");
	}
	else {
		for (i = 1; argc-- > 1; i++) {
			switch (getvfsfile (fp, &vp, argv [i])) {
				case -1:
					rewind (fp);
					break;

				case 0:
					print_vfstab (&vp);
					rewind (fp);
					break;

				default:
					err_quit ("Bad /etc/vfstab file.\n");
					break;
			}
		}
	}

	return (0);
}

static void print_vfstab (struct vfstab *vp)
{
	printf ("Mount point: %s\n", (vp -> vfs_mountp) ?
		vp -> vfs_mountp : "-");
	printf ("  Mounted from: %s\n", (vp -> vfs_special) ?
		vp -> vfs_special : "-");
	printf ("  Device to fsck: %s\n", (vp -> vfs_fsckdev) ?
		vp -> vfs_fsckdev : "-");
	printf ("  File system type: %s\n", (vp -> vfs_fstype) ?
		vp -> vfs_fstype : "-");
	printf ("  Fsck pass number: %s\n", (vp -> vfs_fsckpass) ?
		vp -> vfs_fsckpass : "-");
	printf ("  Mount at boot: %s\n", (vp -> vfs_automnt) ?
		vp -> vfs_automnt : "-");
	printf ("  Mount options: %s\n\n", (vp -> vfs_mntopts) ?
		vp -> vfs_mntopts : "-");
}
