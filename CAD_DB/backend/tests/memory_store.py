"""In-memory JsonGraphStore shared by tests and the native regression server.

Kept free of pytest imports so client_regression_server.py can run under any
interpreter that provides the backend application packages.
"""

from copy import deepcopy
from datetime import UTC, datetime
from uuid import uuid4

from fastapi import HTTPException

from app.json_graph_store import JsonGraphVersion


class MemoryStore:
    def __init__(self):
        self.rows = []
        self.projects = {}

    def create_project(self, name):
        now = datetime.now(UTC)
        from app.neo4j_store import ProjectRecord

        p = ProjectRecord(uuid4().hex, name, now, now)
        self.projects[p.id] = p
        return p

    def get_project_by_name_or_none(self, name):
        return next((p for p in self.projects.values() if p.name == name), None)

    def get_project_or_none(self, pid):
        return self.projects.get(pid)

    def latest(self, pid):
        rows = self.list_after(pid, 0)
        return rows[-1] if rows else None

    def get(self, pid, version):
        return next(
            (v for v in self.rows if v.project_id == pid and v.version == version), None
        )

    def graph(self, pid, version):
        return deepcopy(self.get(pid, version).content["entity_graph"])

    def list_after(self, pid, version):
        return [v for v in self.rows if v.project_id == pid and v.version > version]

    def create(self, pid, author, content, base):
        head = self.latest(pid)
        if base != (head.version if head else None):
            raise HTTPException(409, "stale_base")
        row = JsonGraphVersion(
            pid,
            (head.version if head else 0) + 1,
            author,
            deepcopy(content),
            datetime.now(UTC),
        )
        self.rows.append(row)
        return row
