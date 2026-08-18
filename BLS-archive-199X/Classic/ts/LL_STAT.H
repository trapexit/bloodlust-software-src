/*
**	Incremental Status Header File
*/
#ifndef __STATUS_H__
#define	__STATUS_H__

#define ST_WRITE	0	// File ONLY
#define ST_RESET	1	// Reset FILE
#define ST_CLOSE	2	// Close FILE
#define ST_SHOW 	3	// File & Stderr
#define ST_ECHO 	4	// Stderr ONLY

extern	void	status(int, char *, ...);

#endif
