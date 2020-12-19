// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "corn_test.h"

#include "corn_os/algorithm.h"

TEST(max_min_macro)
{
	int a = 2;
	int b = 3;
	int c = MAX(a, b);
	ASSERT_EQ(c, b);
	int d = ({
		int e = 3;
		e;
	});
	ASSERT_EQ(d, 3);
	d = MIN(a, b);
	ASSERT_EQ(d, a);
	ASSERT_EQ(MAX(a * 10, b + 123), 123 + 3);
}

TEST_MAIN()
