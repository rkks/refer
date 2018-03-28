/*
	Header to generate SMI wrappers
	AUP2, Sec. 7.03.3, 7.05.3, etc.

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
/*
	Generates smi wrapper depending on arg.
	To be included by smi_skt_wrap.c, smi_fifl_wrap.c, etc.
*/

#define wrapper(sfx) \
SMIQ *smi_open(const char *name, SMIENTITY entity, size_t msgsize)\
{\
	return smi_open_##sfx(name, entity, msgsize);\
}\
\
bool smi_close(SMIQ *sqp)\
{\
	return smi_close_##sfx(sqp);\
}\
\
bool smi_send_getaddr(SMIQ *sqp, struct client_id *client, void **addr)\
{\
	return smi_send_getaddr_##sfx(sqp, client, addr);\
}\
\
bool smi_send_release(SMIQ *sqp)\
{\
	return smi_send_release_##sfx(sqp);\
}\
\
bool smi_receive_getaddr(SMIQ *sqp, void **addr)\
{\
	return smi_receive_getaddr_##sfx(sqp, addr);\
}\
\
bool smi_receive_release(SMIQ *sqp)\
{\
	return smi_receive_release_##sfx(sqp);\
}
