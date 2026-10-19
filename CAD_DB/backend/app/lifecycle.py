"""Application lifecycle and rotating error log setup."""

import logging
from contextlib import asynccontextmanager
from logging.handlers import RotatingFileHandler
from pathlib import Path
from fastapi import FastAPI
from . import neo4j_webui
from .v1_store import initialize_v1_store, shutdown_v1_store

logger = logging.getLogger(__name__)
LOG_FILE_PATH = Path(__file__).resolve().parent.parent / "logs" / "backend-error.log"


def _setup_file_logging() -> None:
    log_dir = LOG_FILE_PATH.parent
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = LOG_FILE_PATH

    formatter = logging.Formatter("%(asctime)s %(levelname)s [%(name)s] %(message)s")
    file_handler = RotatingFileHandler(
        log_file, maxBytes=2 * 1024 * 1024, backupCount=3, encoding="utf-8"
    )
    file_handler.setFormatter(formatter)
    file_handler.setLevel(logging.INFO)

    root_logger = logging.getLogger()
    root_logger.setLevel(logging.INFO)
    has_same_handler = any(
        isinstance(h, RotatingFileHandler)
        and getattr(h, "baseFilename", "") == str(log_file.resolve())
        for h in root_logger.handlers
    )
    if not has_same_handler:
        root_logger.addHandler(file_handler)


@asynccontextmanager
async def lifespan(app: FastAPI):
    _setup_file_logging()
    # v1 store：Neo4j 不可用时降级内存实现（本地/单测）；生产配 Neo4j 环境变量。
    try:
        initialize_v1_store()
    except Exception as exc:  # noqa: BLE001 - startup resilience for local runs
        logger.warning("v1 store falling back to memory backend: %s", exc)
        initialize_v1_store(memory=True)
    logger.info("Backend startup completed, log file: %s", LOG_FILE_PATH)
    try:
        yield
    finally:
        shutdown_v1_store()
        neo4j_webui._close_driver()
