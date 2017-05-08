# BBDecTreeTool without Gaudi

If on lxplus running `lb-run ROOT bash --norc` is sufficient to setup the dependencies.

```bash
git clone https://github.com/chrisburr/lhcb-local-trigger-bdt.git
cd lhcb-local-trigger-bdt
mkdir build
cd build
cmake  -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc ..  # Compiler must be specified on lxplus
make
./run_bdt
```

See `src/main.cpp` for configuring. The ntuple used for the example is created using:

```bash
lhcb-proxy-init
lb-run DaVinci/latest gaudirun.py options/options.py
```
