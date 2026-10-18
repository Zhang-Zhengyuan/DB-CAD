from pathlib import Path

from pydantic import Field, field_validator
from pydantic_settings import BaseSettings, SettingsConfigDict

_REPO_ROOT = Path(__file__).resolve().parents[2]


class Settings(BaseSettings):
    app_name: str = "CAD DB Store API"
    storage_backend: str = Field(default="neo4j")
    neo4j_uri: str = Field(default="")
    neo4j_user: str = Field(default="neo4j")
    neo4j_password: str = Field(default="")
    neo4j_database: str = Field(default="neo4j")
    api_password: str = Field(default="")
    # v1（V3_PLAN.md §1）：不可变组件内容寻址根目录 + 几何 worker 可执行文件。
    component_root: str = Field(default=str(Path(__file__).resolve().parent.parent / "data" / "components"))
    worker_exe: str = Field(default=str(_REPO_ROOT / "x64" / "Debug" / "dbcad_worker.exe"))

    @field_validator("storage_backend")
    @classmethod
    def validate_storage_backend(cls, value: str) -> str:
        if value.lower() != "neo4j":
            raise ValueError("Only neo4j storage backend is supported")
        return "neo4j"

    model_config = SettingsConfigDict(
        env_prefix="CAD_DB_", env_file=".env", extra="ignore"
    )


settings = Settings()
