#!/usr/bin/env sh

echo '=====unit test====='
make test
echo '===unit test done==='

echo '=====qemu test====='

test_all_files() {
  file_list=$(ls $1)
  for file in $file_list; do
    if [ -d $1"/"$file ]; then
      test_all_files $1"/"$file
    else
      make TEST=$1"/"$file
      qemu-system-i386 -hda ./bin/corn.img
    fi
  done
}

test_all_files test/qemu
echo '===qemu test done==='
