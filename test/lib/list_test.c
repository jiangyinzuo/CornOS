// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "corn_test.h"

#include "corn_os/list.h"

TEST(list)
{
	LIST_HEAD(elem1);
	ASSERT_EQ(elem1.prev, elem1.next);
	struct list_head elem2;
	INIT_LIST_HEAD(&elem2);
	list_add(&elem1, &elem2);
	ASSERT_EQ(elem1.next, &elem2);
	ASSERT_EQ(elem1.prev, &elem2);
}

TEST_MAIN()
