/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _REVERSE_H_RPCGEN
#define _REVERSE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct arg {
	char str[40];
};
typedef struct arg arg;

#define REVERSE 0x11335577
#define REVVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define revfunc 1
extern  arg * revfunc_1(arg *, CLIENT *);
extern  arg * revfunc_1_svc(arg *, struct svc_req *);
extern int reverse_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define revfunc 1
extern  arg * revfunc_1();
extern  arg * revfunc_1_svc();
extern int reverse_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_arg (XDR *, arg*);

#else /* K&R C */
extern bool_t xdr_arg ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_REVERSE_H_RPCGEN */
