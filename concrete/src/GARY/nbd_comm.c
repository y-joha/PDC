#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>

#include "nbd_comm.h"
/*******Defines******/

#define NBD_BLOCK 1024

/*******Structs******/

typedef struct
{
    NbdRequest          nbd_req;
    int                 nbd_sock;
    struct nbd_reply    reply;
    char                buffer[1];
} nbd_message_t;

/*******Service Funcs******/


static int read_all(int fd, char* buf, size_t count);

static int write_all(int fd, char* buf, size_t count);

u_int64_t ntohll(u_int64_t a);

void CleanupService(int nbd_fd, int scp);


/*******Funcs Funcs******/ 


int NbdDevOpen(const char *dev_name, uint64_t size)
{
    
    int sckt_pair[2];
    int nbd_fd, sckt_num, err_fd;
    pid_t pid = 0;
    
    /* write function for the socketpair */
    
    /* perform ioctl functions */

    do
    {
        err_fd = socketpair(AF_UNIX, SOCK_STREAM, 0, sckt_pair);
        if(-1 == err_fd){break;}
        nbd_fd = open(dev_name, O_RDWR);
        if (nbd_fd == -1) 
        {
            fprintf(stderr, 
            "Failed to open `%s': %s\n"
            "Is kernel module `nbd' loaded and you have permissions"
            "to access the device?\n", dev_name, strerror(errno));
            break;
        }
        err_fd = ioctl(nbd_fd, NBD_CLEAR_SOCK);
        if(-1 == err_fd){break;}
        err_fd = ioctl(nbd_fd, NBD_CLEAR_QUE);
        if(-1 == err_fd){break;}
        if (size) 
        {
            err_fd = ioctl(nbd_fd, NBD_SET_SIZE, size);
        }
        if(-1 == err_fd){break;}
        err_fd = ioctl(nbd_fd, NBD_SET_BLKSIZE, NBD_BLOCK);
        if(-1 == err_fd){break;}
        sckt_num = sckt_pair[1];
        err_fd = ioctl(nbd_fd, NBD_SET_SOCK, sckt_num);
        if(-1 == err_fd){break;}
        if(-1 == (pid = fork())){break;}
        if(0 == pid)
        {
            close(sckt_pair[0]);
            /*printf("NBD DO IT DO\n"); */
            err_fd = ioctl(nbd_fd, NBD_DO_IT); /* blocking */
            if(-1 == err_fd){perror("Failed to DO IT");printf("Do It Is A Bitch And Left Stam");break;}
        }

        close(sckt_pair[1]);
        sckt_num = sckt_pair[0];
        return sckt_num;
    }
    while(0);
    
    /*perror("prior to CleanUp");
    printf("did you get here? %d %d %d\n",__LINE__,getppid() ,getpid());
    */


    CleanupService(nbd_fd,sckt_num);
    return -1;    
}

NbdRequest *NbdRequestRead(int sock)
{
    int err_chk = 0;
    nbd_message_t *msg = NULL;
    struct nbd_request loc_cpy = { 0 };
    
    /* this is a request from the fs side to nbd */
    err_chk = read_all(sock, ((char *)&loc_cpy), sizeof(loc_cpy));
    if(err_chk)
    {
        return NULL;
    }

    msg = (nbd_message_t *)malloc(offsetof(nbd_message_t,buffer) + ntohl(loc_cpy.len));

    msg->reply.magic = htonl(NBD_REPLY_MAGIC);
    msg->reply.error = htonl(0);
    memcpy(msg->reply.handle,loc_cpy.handle,sizeof(loc_cpy.handle));


    msg->nbd_req.dataBuf = msg->buffer;
    msg->nbd_sock = sock;
    
    
    /* read first request for type & size */
    msg->nbd_req.dataLen = ntohl(loc_cpy.len);
    msg->nbd_req.offset = ntohll(loc_cpy.from);
    msg->nbd_req.reqType = ntohl(loc_cpy.type);
    
    if(NBD_CMD_WRITE == msg->nbd_req.reqType)
    {
        read_all(sock,msg->buffer,msg->nbd_req.dataLen);
    }

    return (NbdRequest *)msg;
}

void NbdRequestDone(NbdRequest *req, int error)
{
    nbd_message_t *reply_msg = NULL;
    size_t rply_len = sizeof(struct nbd_reply);

    reply_msg = (nbd_message_t *)req;
    reply_msg->reply.error = htonl(error);
    
    printf("\n\n\n\nNBD Request Done\n\n\n\n");
    
    if(NBD_CMD_READ == reply_msg->nbd_req.reqType)
    {
        rply_len += reply_msg->nbd_req.dataLen;
    }
    write_all(reply_msg->nbd_sock,(char *)&reply_msg->reply,rply_len);
    free(reply_msg);
}


static int read_all(int fd, char* buf, size_t count)
{
  int bytes_read;

  while (count > 0) {
    bytes_read = read(fd, buf, count);
    assert(bytes_read > 0);
    buf += bytes_read;
    count -= bytes_read;
  }
  assert(count == 0);

  return 0;
}

static int write_all(int fd, char* buf, size_t count)
{
  int bytes_written;

  while (count > 0) {
    bytes_written = write(fd, buf, count);
    assert(bytes_written > 0);
    buf += bytes_written;
    count -= bytes_written;
  }
  assert(count == 0);

  return 0;
}

void CleanupService(int nbd_fd, int scp)
{
    if (ioctl(nbd_fd, NBD_CLEAR_QUE) == -1 ||
            ioctl(nbd_fd, NBD_CLEAR_SOCK) == -1) 
    {
        perror("failed to perform nbd cleanup actions");
    }
    
    close(nbd_fd);
    close(scp);
}


u_int64_t ntohll(u_int64_t a) {
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
