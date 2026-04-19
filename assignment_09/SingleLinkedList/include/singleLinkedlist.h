#ifndef SINGLELINKEDLIST_H_
#define SINGLELINKEDLIST_H_

typedef struct Person Person;

/**
 * @brief Insert node at list head.
 * Inserts the node _p at the beginning of the list.
 * @param _head Current head of the list.
 * @param _p Node to insert.
 *
 * @return New head of the list.
 * @retval _p     If insertion is performed.
 * @retval _head  If _p is NULL.
 * 
 * 
 * @note This function does not allocate memory.
 *       It only updates pointers.
 *
 * @warning If _p is already linked to another list, 
 *          its next pointer will be overwritten.
 */
Person* ListInsertHead(Person* _head ,Person* _p);

/**
 * @brief Removes the first node from the list.
 *
 * Removes the head node from the list and returns it through _item.
 *
 * @param _head Current head of the list.
 * @param _item Output parameter that receives the removed node.
 *
 * @return Pointer to the new head of the list.
 *
 * @retval Updated head of the list after removal.
 * @retval _head If the list is empty or if _item is NULL.
 *
 * @note The removed node is detached from the list.
 * @note This function does not free memory.
 */
Person* ListRemoveHead(Person* _head, Person** _item);


/**
 * @brief Insert node into sorted list by key.
 *
 * Inserts `_p` into a sorted list while preserving order (ascending by m_id).
 * Duplicate keys are not allowed.
 *
 * @param _head Current list head.
 * @param _key  Key for insertion (must match _p->m_id).
 * @param _p    Node to insert.
 *
 * @return Updated list head.
 */
Person* ListInsertByKey(Person* _head, int _key, Person* _p);

//Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p);


/**
 * @brief Remove node by key.
 *
 * Removes the node with matching `_key` from the list and returns it via `_p`.
 * If not found, `_p` is set to NULL and the list is unchanged.
 *
 * @param _head List head.
 * @param _key  Key to remove.
 * @param _p    Output removed node.
 *
 * @return Updated list head.
 */
Person* ListRemoveByKey(Person* _head, int _key, Person* *_p);

//Person* ListRemoveByKeyRec(Person* _head, int _key, Person**_p);

/**
 * @brief Print all elements in the list.
 *
 * @param _head List head.
 *
 * @note Does nothing if list is empty.
 */
void PrintList(Person* _head);

/**
 * @brief Create a new Person node.
 *
 * @param _id   Person ID.
 * @param _name Person name (must not be NULL).
 * @param _age  Person age.
 *
 * @return New Person pointer, or NULL on failure.
 *
 * @note Caller must free using PersonDestroy().
 */
Person* PersonCreate(int _id, const char* _name, int _age);

/**
 * @brief Free a Person node.
 *
 * @param _p Person to destroy (NULL is safe).
 *
 * @note Does not affect other nodes.
 */
void PersonDestroy(Person* _p);

#endif // SINGLELINKEDLIST_H_