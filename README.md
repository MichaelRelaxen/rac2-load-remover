# gcpt

Work in progress practice tool for Ratchet & Clank 2: Going Commando on da PlayStation Double.

To build, setup docker
```docker run -it --rm -v "$PWD\:/src" ps2dev/ps2dev:v1.2.0
cd src
cd gcpt
./docker-init.sh
```

Then use:
```make clean && make```
