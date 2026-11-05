# Build tools only. Mount authorized SDK/license inputs at runtime; never COPY them.
FROM ubuntu:22.04
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
       build-essential python3 git ca-certificates \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
ENTRYPOINT ["python3", "scripts/linux-worker.py"]
