/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/Action.h $
**	$Author: nico $
**	$Revision: 361 $
**	$Date: 2005-12-06 15:52:07 +0100 (Tue, 06 Dec 2005) $
**
*/

#ifndef _ACTIONS_H_
# define _ACTIONS_H_

//
// Core
//
# define CORE_DELETE_TICKET		-1
# define CORE_TICKET_DELETED	-2
# define CORE_RELOAD_ALL	    -3
# define CORE_HALT_MODULE		-4
# define CORE_HALTED_MODULE		-5
//
// Network
//
# define NETWORK_SENDDATA		1
# define NETWORK_NEWCLIENT		2
# define NETWORK_TOANSWERING	4
# define NETWORK_READ			5
# define NETWORK_CLEAN		    6
//
// Listener
//
# define LISTENER_OPENPORT		20
# define LISTENER_CLOSEPORT		21
//
// Answering
//
# define ANSWERING_IDROPTOYOU	41
//
// Protocol
//
# define PROTOCOL_NEWDATA		60
//
// ListDir
//
# define LISTDIR_BAZOU			80
//
// FileReg
//
# define FILEREG_SENDFILE		120
//
// Common
//
# define NEWDATA				100
# define NEWBUFFER				101
# define RELEASE_TICKET			102
//
// FileReg
//
# define FILEREG_SENDFILE		120
//
// CGI
//
# define CGI_PROCESS			140      

#endif /* !_ACTIONS_H_ */
