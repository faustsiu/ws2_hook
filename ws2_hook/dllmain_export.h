typedef int (CALLBACK* LPFNDLLFUNCCONNECT)(SOCKET s, const struct sockaddr *name, int namelen);
typedef int (CALLBACK* LPFNDLLFUNCSEND)(SOCKET s, const char *buf, int len, int flags);
typedef int (CALLBACK* LPFNDLLFUNCRECV)(SOCKET s, char *buf, int len, int flags);

extern LPFNDLLFUNCCONNECT ws2_connect;
extern LPFNDLLFUNCSEND ws2_send;
extern LPFNDLLFUNCRECV ws2_recv;

void wf_flush (void);
int wf_send (SOCKET s, const char *buf, int len, int flags);