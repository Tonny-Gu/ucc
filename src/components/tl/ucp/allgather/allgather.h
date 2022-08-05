/**
 * Copyright (C) Mellanox Technologies Ltd. 2021.  ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */
#ifndef ALLGATHER_H_
#define ALLGATHER_H_
#include "../tl_ucp.h"
#include "../tl_ucp_coll.h"

ucc_status_t ucc_tl_ucp_allgather_init(ucc_tl_ucp_task_t *task);
ucc_status_t ucc_tl_ucp_allgather_ring_init(ucc_base_coll_args_t *coll_args,
                                             ucc_base_team_t      *team,
                                             ucc_coll_task_t     **task_h);
ucc_status_t ucc_tl_ucp_allgather_ring_init_common(ucc_tl_ucp_task_t *task);
ucc_status_t ucc_tl_ucp_allgather_ring_progress(ucc_coll_task_t *task);
ucc_status_t ucc_tl_ucp_allgather_ring_start(ucc_coll_task_t *task);

/* Uses allgather_kn_radix from config */
ucc_status_t ucc_tl_ucp_allgather_knomial_init(ucc_base_coll_args_t *coll_args,
                                               ucc_base_team_t *     team,
                                               ucc_coll_task_t **    task_h);

/* Internal interface with custom radix */
ucc_status_t ucc_tl_ucp_allgather_knomial_init_r(
    ucc_base_coll_args_t *coll_args, ucc_base_team_t *team,
    ucc_coll_task_t **task_h, ucc_kn_radix_t radix);

#ifdef HAVE_DPU_OFFLOAD
/*
 * Collective data structure that needs to be maintained through the life cycle
 * of the collective operation, make sure to release all temporary data
 * properly when collective is completed.
 */
typedef struct allgather_host_coll {
    ucs_list_link_t super;          /* op item */
    ucc_coll_task_t *coll_task;     /* collective task */
    int complete;                   /* completion flag */
    ucp_mem_h s_memh;               /* send memh */
    ucp_mem_h r_memh;               /* receive memh */
} allgather_host_coll_t;

/* active collectives running on the HOST */
ucs_list_link_t active_colls;

enum {
    UCC_TL_UCP_ALLGATHER_ALG_RING,
    UCC_TL_UCP_ALLGATHER_ALG_OFFLOAD,
    UCC_TL_UCP_ALLGATHER_ALG_LAST
};

extern ucc_base_coll_alg_info_t
    ucc_tl_ucp_allgather_algs[UCC_TL_UCP_ALLGATHER_ALG_LAST + 1];

#define UCC_TL_UCP_ALLGATHER_DEFAULT_ALG_SELECT_STR "allgather:0-inf:@ring"

ucc_status_t ucc_tl_ucp_allgather_offload_init(ucc_base_coll_args_t *coll_args,
                                                ucc_base_team_t      *team,
                                                ucc_coll_task_t     **task_h);
ucc_status_t ucc_tl_ucp_allgather_offload_start(ucc_coll_task_t *task);
ucc_status_t ucc_tl_ucp_allgather_offload_progress(ucc_coll_task_t *task);

static inline int ucc_tl_ucp_allgather_alg_from_str(const char *str)
{
    int i;
    for (i = 0; i < UCC_TL_UCP_ALLGATHER_ALG_LAST; i++) {
        if (0 == strcasecmp(str, ucc_tl_ucp_allgather_algs[i].name)) {
            break;
        }
    }
    return i;
}
#endif // HAVE_DPU_OFFLOAD

#endif
