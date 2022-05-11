typedef struct deque_entry {
    int data;
    struct deque_entry *next;
    struct deque_entry *prev;
} deque_entry;

typedef struct deque {
    deque_entry *front;
    deque_entry *back;
} deque;

void dq_init(deque *dq)
{
    dq->front = NULL;
    dq->back = NULL;
}

void dq_init_with_entry(deque *dq, deque_entry *entry)
{
    dq->front = entry;
    dq->back = entry;
}

int dq_is_empty(deque *dq)
{
    return dq->front == NULL && dq->back == NULL;
}
deque_entry* dq_alloc_entry(int data)
{
    deque_entry *entry = malloc(sizeof(*entry));
    if (entry != NULL) {
        entry->data = data;
        entry->next = NULL;
        entry->prev = NULL;
    }
    return entry;
}
int dq_push_front(deque *dq, int data)
{
    deque_entry *entry = dq_alloc_entry(data);
    if (entry == NULL) {
        return ENOMEM;
    }

    /* empty deque, front and back will point to the new element */
    if (dq_is_empty(dq)) {
        dq_init_with_entry(dq, entry);
        return 0;
    }

    entry->next = dq->front;
    dq->front->prev = entry;
    dq->front = entry;
    return 0;
}

int dq_push_back(deque *dq, int data)
{
    deque_entry *entry = dq_alloc_entry(data);
    if (entry == NULL) {
        return ENOMEM;
    }

    if (dq_is_empty(dq)) {
        dq_init_with_entry(dq, entry);
        return 0;
    }

    entry->prev = dq->back;
    dq->back->next = entry;
    dq->back = entry;
    return 0;
}
int dq_pop_front(deque *dq, int *data)
{
    deque_entry *entry;

    if (dq_is_empty(dq)) {
        return EINVAL;
    }

    entry = dq->front;
    dq->front = entry->next;

    /* last element popped */
    if (dq->front == NULL) {
        dq_init(dq);
    } else {
        dq->front->prev = NULL;
    }

    *data = entry->data;
    free(entry);
    return 0;
}

int dq_pop_back(deque *dq, int *data)
{
    deque_entry *entry;

    if (dq_is_empty(dq)) {
        return EINVAL;
    }

    entry = dq->back;
    dq->back = entry->prev;

    /* last element popped */
    if (dq->back == NULL) {
        dq_init(dq);
    } else {
        dq->back->next = NULL;
    }

    *data = entry->data;
    free(entry);
    return 0;
}
int dq_peek_front(deque *dq, int *data)
{
    if (dq_is_empty(dq)) {
        return EINVAL;
    }

    *data = dq->front->data;
    return 0;
}

int dq_peek_back(deque *dq, int *data)
{
    if (dq_is_empty(dq)) {
        return EINVAL;
    }

    *data = dq->back->data;
    return 0;
}
void dq_cleanup(deque *dq)
{
    int data;
    while (!dq_is_empty(dq)) {
        dq_pop_front(dq, &data);
    }
}
void dq_dump_reverse(deque *dq)
{
    deque_entry *entry = dq->back;
    printf("List contents (dir = reverse): ");
    while (entry) {
        printf("(%d) -> ", entry->data);
        entry = entry->prev;
    }
    printf("\n\n");
}

void dq_dump(deque *dq)
{
    deque_entry *entry = dq->front;
    printf("List contents (dir = forward): ");
    while (entry) {
        printf("(%d) -> ", entry->data);
        entry = entry->next;
    }
    printf("\n");
    dq_dump_reverse(dq);
}

int main()
{
    deque dq;
    run_tests(&dq);
    return 0;
}
