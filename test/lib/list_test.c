// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "../corn_test.h"

#include "corn_os/list.h"

TEST(list_add)
{
	LIST_HEAD(head);

	ASSERT_EQ(head.prev, head.next);

	struct list_head elem1;
	INIT_LIST_HEAD(&elem1);
	LIST_HEAD(elem2);

	list_add(&head, &elem1);
	ASSERT_EQ(head.prev, &elem1);
	ASSERT_EQ(head.next, &elem1);
	ASSERT_EQ(elem1.prev, &head);
	ASSERT_EQ(elem1.next, &head);

	list_add_tail(&elem2, &elem1);
	ASSERT_EQ(head.next, &elem2);
	ASSERT_EQ(elem2.next, &elem1);
	ASSERT_EQ(elem2.prev, &head);

	ASSERT(list_is_first(&head, &elem1));
	ASSERT(list_is_last(&head, &elem2));
}

struct foo_t {
	int a;
	char b;
	struct list_head *list_head;
};

TEST(list_entry)
{
	LIST_HEAD(head);
	struct foo_t foo = { 1, 'a', &head };
	struct foo_t *foo_ptr =
		list_entry(&foo.list_head, struct foo_t, list_head);
	ASSERT_EQ(&foo, foo_ptr);
	ASSERT_EQ(1, foo_ptr->a);
	//	printf("%lu\n", &foo);
	//	printf("%lu\n", &head);
	//	printf("%lu\n", &foo.list_head);
	//	printf("%lu\n", foo_ptr);
	//	printf("%d\n", foo_ptr->a);
	//	printf("%d\n", offsetof(struct foo_t, list_head));
}

TEST_MAIN()
