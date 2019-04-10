/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL)
        return NULL;

    INIT_LIST_HEAD(&q->head);
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL)
        return;
    list_ele_t *entry, *safe;

    list_for_each_entry_safe(entry, safe, &q->head, list)
    {
        free(entry->value);
        list_del(&entry->list);
        free(entry);
    }
    free(q);
}
/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL || s == NULL)
        return false;

    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    char *t = malloc(strlen(s) + 1);
    if (t == NULL) {
        free(newh);
        return false;
    }

    strcpy(t, s);
    newh->value = t;

    list_add(&newh->list, &q->head);
    q->size++;
    return true;
}

/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL || s == NULL)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    char *t = malloc(strlen(s) + 1);
    if (t == NULL) {
        free(newh);
        return false;
    }
    strcpy(t, s);
    newh->value = t;

    list_add_tail(&newh->list, &q->head);
    q->size++;
    return true;
}
/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (q == NULL || q->size == 0)
        return false;

    list_ele_t *node = list_first_entry(&q->head, list_ele_t, list);
    list_del(&node->list);

    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    free(node->value);
    free(node);
    q->size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return q == NULL ? 0 : q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size == 0)
        return;

    list_ele_t *entry, *safe;
    list_for_each_entry_safe(entry, safe, &q->head, list)
    {
        entry->list.next = entry->list.prev;
        entry->list.prev = &safe->list;
    }
    /* You need to write the code for this function */
    q->head.next = q->head.prev;
    q->head.prev = &safe->list;
}

/*
 * merge sort
 */
bool compare(char *s, char *t)
{
    int i;
    for (i = 0; s[i] != '\0' && t[i] != '\0'; i++) {
        if (s[i] != t[i])
            break;
    }
    return s[i] <= t[i];
}
void sort(queue_t *q)
{
    merge_sort(&q->head, q->size);
}
void merge_sort(struct list_head *head, int size)
{
    if (size == 1) {  // only one element left
        return;
    }
    struct list_head *left, *right;
    struct list_head *node = head;

    left = malloc(sizeof(struct list_head));
    right = malloc(sizeof(struct list_head));
    // find the cutting point
    for (int i = 0; i < size / 2; i++)
        node = node->next;

    list_cut_position(left, head, node);
    list_cut_position(right, head, head->prev);

    merge_sort(left, size / 2);
    merge_sort(right, size - size / 2);

    // merge
    INIT_LIST_HEAD(head);
    list_ele_t *a, *b;
    while (!list_empty(left) && !list_empty(right)) {
        a = list_first_entry(left, list_ele_t, list);
        b = list_first_entry(right, list_ele_t, list);

        node = compare(a->value, b->value) ? left->next : right->next;
        list_del(node);
        list_add_tail(node, head);
    }
    list_splice_tail(left, head);
    list_splice_tail(right, head);
}
