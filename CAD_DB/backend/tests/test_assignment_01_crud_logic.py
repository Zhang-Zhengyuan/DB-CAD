from datetime import datetime, timezone

import pytest
from fastapi import HTTPException

from app import crud, schemas
from app.storage_bridge import VersionRecord
from tests.assignment_support import install_memory_storage


def test_create_project_rejects_duplicate_name(monkeypatch: pytest.MonkeyPatch) -> None:
    storage = install_memory_storage(monkeypatch)

    payload = schemas.ProjectCreate(name="demo-project")
    created = crud.create_project(payload)

    assert created.name == "demo-project"
    assert storage.get_project_by_name_or_none("demo-project") is not None

    with pytest.raises(HTTPException) as exc_info:
        crud.create_project(payload)

    assert exc_info.value.status_code == 409
    assert "already exists" in str(exc_info.value.detail)


def test_create_model_version_enforces_base_version_rules(monkeypatch: pytest.MonkeyPatch) -> None:
    storage = install_memory_storage(monkeypatch)
    project = storage.create_project("crud-assignment")

    first_payload = schemas.ModelVersionCreate(author="alice", content={"sat": "v1"}, base_version=None)
    first = crud.create_model_version(project.id, first_payload)
    assert first.version == 1

    missing_base = schemas.ModelVersionCreate(author="alice", content={"sat": "v2"}, base_version=None)
    with pytest.raises(HTTPException) as exc_info:
        crud.create_model_version(project.id, missing_base)
    assert exc_info.value.status_code == 409
    assert "base_version is required" in str(exc_info.value.detail)

    second_payload = schemas.ModelVersionCreate(author="alice", content={"sat": "v2"}, base_version=1)
    second = crud.create_model_version(project.id, second_payload)
    assert second.version == 2

    stale_payload = schemas.ModelVersionCreate(author="alice", content={"sat": "stale"}, base_version=1)
    with pytest.raises(HTTPException) as stale_exc:
        crud.create_model_version(project.id, stale_payload)
    assert stale_exc.value.status_code == 409
    assert "latest version is 2" in str(stale_exc.value.detail)


def test_deserialize_version_maps_version_record(monkeypatch: pytest.MonkeyPatch) -> None:
    install_memory_storage(monkeypatch)
    record = VersionRecord(
        id=7,
        project_id="project-1",
        version=3,
        author="bob",
        content={"sat": "payload"},
        created_at=datetime.now(timezone.utc),
    )

    model = crud.deserialize_version(record)

    assert model.id == 7
    assert model.project_id == "project-1"
    assert model.version == 3
    assert model.author == "bob"
    assert model.content == {"sat": "payload"}


def test_deserialize_version_rejects_unknown_entity(monkeypatch: pytest.MonkeyPatch) -> None:
    install_memory_storage(monkeypatch)

    with pytest.raises(HTTPException) as exc_info:
        crud.deserialize_version(object())

    assert exc_info.value.status_code == 500
    assert "Invalid version entity type" in str(exc_info.value.detail)
