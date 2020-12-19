TEST_SRC := $(shell find . -type f -regex '.*\.[S|c]')
TEST_OBJS := $(filter %.o,$(TEST_SRC:.S=.o) $(TEST_SRC:.c=.o))

GCC_FLAGS := -I. -I../.. -I../../include -Wall

.PHONY: test

test: $(TEST_OBJS)

%.o: %.c
	@mkdir -p $(shell dirname bin/$@)
	@gcc $(GCC_FLAGS) $< -o bin/$(basename $<)
	@./bin/$(basename $<)

clean:
	rm -rf ./bin


all: test
