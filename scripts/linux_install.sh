#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd)"

BUILD_DIR="${REPO_ROOT}/build/release"
INSTALL_DIR="${REPO_ROOT}/build/install"

log()
{
    echo "[$(date -u +%H:%M:%S)] $*"
}

configure()
{
    log "Configuring Release build..."
    log "Build directory: ${BUILD_DIR}"
    log "Install directory: ${INSTALL_DIR}"
    log "Generator: Ninja"
    log "C compiler: clang"
    log "C++ compiler: clang++"

    cmake \
        -S "${REPO_ROOT}" \
        -B "${BUILD_DIR}" \
        -G Ninja \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"

    log "Configuration complete."
}

build()
{
    log "Building Release..."

    cmake --build "${BUILD_DIR}" --parallel

    log "Build complete."
}

install()
{
    log "Installing to ${INSTALL_DIR}..."

    cmake --install "${BUILD_DIR}"

    log "Installation complete."
}

show_install()
{
    log "Installed files:"

    if command -v tree >/dev/null 2>&1; then
        tree "${INSTALL_DIR}"
    else
        find "${INSTALL_DIR}" -type f -print
    fi
}

main()
{
    log "Starting Linux install..."
    log "Repository root: ${REPO_ROOT}"

    configure
    build
    install
    show_install

    log "Linux install finished successfully."
}

main "$@"