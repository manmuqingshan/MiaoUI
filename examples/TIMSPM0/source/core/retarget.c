#include <_ansi.h>

#include <_syslist.h>

#include <errno.h>

#include <sys/time.h>

#include <sys/times.h>

#include <retarget.h>

#include <stdint.h>

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0

#define STDOUT_FILENO 1

#define STDERR_FILENO 2


UART_Regs* gHuart;


void RetargetInit(UART_Regs* huart)
{
    gHuart = huart;
    /* Disable I/O buffering for STDOUT stream, so that

 * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}


int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;
    errno = EBADF;
    return 0;
}


int _write(int fd, char* ptr, int len)
{
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    {
        for (int i = 0; i < len; i++)
        {
            // 阻塞发送一个字节
            DL_UART_Main_transmitData(gHuart, (uint8_t)ptr[i]);
            // 如果硬件需要等待发送完成，这里可能要加等待
            while (DL_UART_isBusy(gHuart) == true);
        }
        return len;
    }
    errno = EBADF;
    return -1;
}


int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;
    errno = EBADF;
    return -1;
}


int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}


int _read(int fd, char* ptr, int len)

{
    if (fd == STDIN_FILENO)
    {
        DL_UART_Main_receiveData(gHuart);
        return 1;
    }
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat* st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }
    errno = EBADF;
    return 0;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)
