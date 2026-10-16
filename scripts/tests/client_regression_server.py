"""Isolated real HTTP server for the native client regression. No user database."""
import sys
from pathlib import Path
from contextlib import asynccontextmanager

backend = Path(__file__).resolve().parents[2] / 'CAD_DB' / 'backend'
sys.path.insert(0, str(backend))
sys.path.insert(0, str(backend / 'tests'))
from test_hybrid_collaboration import MemoryStore
from app import crud
from app.api import json_delta, models
from app.collaboration import snapshots
import app.json_graph_store as persistence
from app.config import settings
from app.main import app
import uvicorn

store = MemoryStore()
crud.direct_store = store
for module in (json_delta, models, snapshots, persistence):
    module.get_json_graph_store = lambda: store
settings.api_password = ''

@asynccontextmanager
async def isolated_lifespan(app):
    yield

app.router.lifespan_context = isolated_lifespan
if __name__ == '__main__':
    uvicorn.run(app, host='127.0.0.1', port=int(sys.argv[1]))
