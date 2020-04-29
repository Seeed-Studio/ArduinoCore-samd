/*
 *  The MIT License (MIT)
 *  Copyright (C) 2019-2020 Seeed Technology Co.,Ltd.
 */
#include "Arduino.h"
#include <errno.h>
#include <sys/reent.h>

#ifdef __cplusplus
extern "C" {
#endif

// libnosys.a provide all the symbols except _write
// with gcc linker option "--specs=nosys.specs --wrap,_write"

#if 0
int _close (int fd) {
	return 0;
}

void _exit (int status) {
	return;
}

int _fstat (int fd, struct stat *st) {
	return 0;
}

int _isatty(int fd) {
	return 1;
}

int _lseek (int file, int ptr, int dir) {
	return 0;
}

int _read (int fd, char *ptr, int len) {
	return len;
}

/*
void _ttywrch(int ch) {
	Serial.write((char)ch); 
	return;
}
*/

int _kill(void) {}

int _getpid(void) {}
#endif

extern int __real__write (int fd, const char *ptr, int len);

int __wrap__write (int fd, const char *ptr, int len) {
	(void)fd;	// UNUSED

	int i;

	if (!Serial) {
		return len;
	}

	for (i = 0; i < len; i++) {
		Serial.write(ptr[i]);
	}
	return len;
}

#ifdef __cplusplus
}
#endif
