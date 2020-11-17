#include <stdio.h>

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

struct UART_Data_ele{	//新增
	unsigned int command[2];//新增
	unsigned char P1[6];//新增
	unsigned char P2[6];//新增
	//struct UART_Data_ele *next; 
};

char *portname_0 = "/dev/ttyUSB0";
int uart_fd_0 = -1;

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error from tcgetattr\r\n");
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error from tcsetattr\r\n");
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error  from tggetattr\r\n");
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error setting term attributes\r\n");
}


/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void *uart_0_worker_thread(void *data) //新創通道
{
	int x=0;

	uart_fd_0 = open (portname_0, O_RDWR | O_NOCTTY | O_SYNC);//O_RDWR讀取寫入權限
	if (uart_fd_0 < 0)
	{
			printf ("uart_0_worker_thread !!!! : error opening %s \r\n",portname_0);
			return;
	}
	set_interface_attribs (uart_fd_0, B115200, 0);//UART設定
	set_blocking(uart_fd_0, 0);//UART設定

	while(1)
	{
		usleep(500000);//500ms
		if ((x % 5) ==0)
		{

			printf("hello world!\r\n");
			printf("哈囉!\r\n");
			
			int free_readdd = 0;
			//unsigned char jjj[1024] = {0};//1024byte 內容全是0
			unsigned int  uart_r_len = read(uart_fd_0, jjj, 1024);//將讀取到寫入uart_r_len
			struct UART_Data_ele m_kk;
			memset(m_kk , 0 , sizeof(struct UART_Data_ele));


			if(uart_r_len > 0)//uart_r_len有寫入
			{
				//write (uart_fd_0, jjj, uart_r_len);//回傳寫入uart_r_len的值
				if (m_kk.command[0] == 'A' && m_kk.command[1] == '1')//新增
				{//新增
					printf("收到主控A1\r\n")//新增
				}//新增
				//printf("send!\r\n");
			}

		}
		x++;
		if (x>4096)
		{
			x=0;
		}
	}

}


int main(void)
{
	int y = 0;
	pthread_t thread_0;
	pthread_create(&thread_0, NULL, &uart_0_worker_thread, NULL); //開新通道

	while(1)
	{
		usleep(2000000);//2sec
		if ((y % 5) ==0)
		{




			printf("hd!\r\n");
			printf("哈哈!\r\n");
		
		}
		y++;
		if (y>4096)
		{
			y=0;
		}
	}
	return 0;
}


