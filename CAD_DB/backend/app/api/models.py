"""Mode0 snapshot version endpoints."""

from fastapi import APIRouter, Depends, Query, Request

from .. import crud, schemas


from ..dependencies import verify_api_password

router = APIRouter()

from ..services.model_versions import _create_model_version_serialized


@router.post(
    "/projects/{project_id}/models", response_model=schemas.SaveResult, status_code=201
)
async def save_model(
    project_id: str,
    payload: schemas.ModelVersionCreate,
    request: Request,
    _: None = Depends(verify_api_password),
) -> schemas.SaveResult:
    raw_request_json = (await request.body()).decode("utf-8")
    version = await _create_model_version_serialized(
        project_id, payload, trigger="http_save", raw_request_json=raw_request_json
    )
    return schemas.SaveResult(version=version.version, created_at=version.created_at)


@router.get(
    "/projects/{project_id}/models/latest", response_model=schemas.ModelVersionRead
)
def get_latest_model(
    project_id: str,
    _: None = Depends(verify_api_password),
) -> schemas.ModelVersionRead:
    version = crud.get_latest_version_or_404(project_id)
    return crud.deserialize_version(version)


@router.get(
    "/projects/{project_id}/models/versions",
    response_model=list[schemas.ModelVersionRead],
)
def get_versions(
    project_id: str,
    limit: int = Query(default=50, ge=1, le=200),
    offset: int = Query(default=0, ge=0),
    _: None = Depends(verify_api_password),
) -> list[schemas.ModelVersionRead]:
    versions = crud.list_versions(project_id, limit=limit, offset=offset)
    return [crud.deserialize_version(item) for item in versions]


@router.get(
    "/projects/{project_id}/models/{version}", response_model=schemas.ModelVersionRead
)
def get_model_by_version(
    project_id: str,
    version: int,
    _: None = Depends(verify_api_password),
) -> schemas.ModelVersionRead:
    entity = crud.get_version_or_404(project_id, version)
    return crud.deserialize_version(entity)
