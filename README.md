![GitHub](https://img.shields.io/github/license/ChiangYintso/CornOS)
![build](https://github.com/ChiangYintso/CornOS/workflows/build/badge.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/ChiangYintso/CornOS)

# CornOS

A toy OS written in C

## Run In QEMU

Run in QEMU

```shell
$ make clean
$ make
$ ./start.sh
```

Run QEMU test

```shell
$ ./start.sh
```

Run unit test

```shell
$ make test
```

Debug in GDB

```shell
$ ./dbg.sh
```

## Release Note

### 0.2.x

- clock interrupt

### 0.1.x

- text mode IO

### mbr-0.0.x
- master boot record
- boot loader

## Reference
[REFERENCE.md](./REFERENCE.md)
