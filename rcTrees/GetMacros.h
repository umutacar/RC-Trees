#ifndef __GET_MACROS_H
#define __GET_MACROS_H

#define GET_CL(c) (cluster *)((intptr_t) c & ~3)
#define GET_BC(c) (bin_cluster *)((intptr_t) c & ~3)

#define GET_DT(c) (bin_data *)((intptr_t) c & ~3)
#define GET_IN(c) (int) ((intptr_t) c & 1)
#define GET_OFF(c) (int) ((intptr_t) c & 1)
#define GET_OT(c) (int) (~((intptr_t)c & 1) & 1)

#define PURIFY(c) (*c = (cluster*) (((intptr_t) (*c)) & ~3))

#define THIS_DATA(d) ((GET_DT(d))->data + (GET_IN(d)))
#define OTHER_DATA(d) ((GET_DT(d))->data + (GET_OT(d)))

#define GET_CLUSTER_DATA(cl) ((GET_BC(cl))->data[GET_IN(cl)])
#define GET_OTHER_DATA(cl) ((GET_CL(cl))->data[GET_OT(cl)])
#define BIN_DATA(bc) (bin_data*)((((intptr_t) &(GET_BC(bc))->data))| (GET_OFF(bc))) 

#endif
