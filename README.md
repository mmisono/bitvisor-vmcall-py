# bitvisor-vmcall-py

## Compile
```
git clone htts://github.com/mmisono/bitvisor-vmcall-py --recursive
cd bitvisor-vmcall-py
mkdir build
cd build
cmake ..
make
```

install `vmcall.xxx.so` in your `PYTHONPATH` or you can add path in the script
using `sys.path.append()`, or you can put `vmcall.xx.so` in the directory same as the script.

## TODO
- AMD support
- Ensure buffer is never swapped out
