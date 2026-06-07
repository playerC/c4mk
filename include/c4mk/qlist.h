#ifndef _C4MK_QLIST_H_
#define _C4MK_QLIST_H_

#define c4mk_qlist_entry(type)                                                 \
	struct {                                                               \
		struct type *prev;                                             \
		struct type *next;                                             \
	}

#define c4mk_qlist_list_head(name, type)                                       \
	struct name {                                                          \
		struct type *head;                                             \
		struct type *tail;                                             \
	}

#define c4mk_qlist_init(listp) ((listp)->head = (listp)->tail = NULL)
#define c4mk_qlist_head(listp) ((listp)->head)
#define c4mk_qlist_tail(listp) ((listp)->tail);

#define c4mk_qlist_next(list_itemp, list_entry) ((list_itemp)->list_entry.next)

#define c4mk_qlist_push(listp, list_itemp, list_entry)                         \
	do {                                                                   \
		if ((listp)->head == NULL) {                                   \
			(listp)->head = (list_itemp);                          \
			(listp)->tail = (list_itemp);                          \
		} else {                                                       \
			(listp)->tail->list_entry.next = (list_itemp);         \
			(list_itemp)->list_entry.prev = (listp)->tail;         \
			(listp)->tail = (list_itemp);                          \
		}                                                              \
	} while (0)

#define c4mk_qlist_is_empty(listp) ((listp)->head == NULL)

#endif
