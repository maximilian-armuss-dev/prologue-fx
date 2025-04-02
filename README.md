# Useful Pages

* https://korginc.github.io/logue-sdk/ref/prologue/v1.1-0/html/index.html
* https://github.com/korginc/logue-sdk/tree/master/platform/prologue

# Setup
* `git clone --recurse-submodules git@github.com:maximilian-armuss-dev/prologue-fx.git`
* `git submodule update --init --recursive`

# Building
1. Start docker desktop (confirm via `docker info`)
2. Build:
``docker build -t logue-sdk-dev-env -f docker/docker-app/Dockerfile docker/docker-app``
3. Start interactive shell: `docker/run_interactive.sh`
4. (Optional) List all buildable projects: `build -l --prologue`
5. Build a project, e.g.: `build prologue/pan-modfx`
