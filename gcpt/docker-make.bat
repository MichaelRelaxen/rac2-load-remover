@echo off
set DOCKER_IMAGE=ps2dev/ps2dev:v1.2.0
docker run -t --rm -v "%CD%:/src" "%DOCKER_IMAGE%" sh -c "cd src; cd gcpt; ./docker-init.sh; make clean; make"